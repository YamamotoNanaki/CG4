#include "Enemy.h"
#include "IFETime.h"
#include "Transform.h"
#include "Collider.h"
#include "Bullet.h"
#include "Object3D.h"
#include "Material.h"
#include "TransferGeometryBuffer.h"
#include "ParticleManager.h"
#include "FireworkChrysanthemum.h"
#include "Ease.h"
#include "Player.h"
#include "SpiritFire.h"
#include "LightManager.h"

using namespace IFE;

Transform* Enemy::playerTransform_ = nullptr;

void IFE::Enemy::Initialize()
{
	sScore_ = 0;
	if (patrolPoint_.size() > 0)
	{
		action_ = (uint8_t)EnemyAction::Patrol;
	}
	else
	{
		action_ = (uint8_t)EnemyAction::Stanby;
	}
	if (transform_ != nullptr && action_ == (uint8_t)EnemyAction::Patrol)
	{
		transform_->position_ = patrolPoint_[0];
	}
	if (objectPtr_->GetComponent<TransferGeometryBuffer>())
	{
		objectPtr_->GetComponent<TransferGeometryBuffer>()->bllomFlag_ = true;
	}
	if (objectPtr_->GetComponent<Material>())
	{
		objectPtr_->GetComponent<Material>()->bloom_ = true;
	}
	if (!visual_)
	{
		auto spirit = ParticleManager::Instance()->Instantiate("SpiritFire");
		if (spirit)
		{
			spirit->GetComponent<SpiritFire>()->SetEnemy(objectPtr_);
			visual_ = true;
		}
	}
	objectPtr_->isActive_ = false;
}

void IFE::Enemy::Update()
{
	objectPtr_->DrawFlag_ = false;
	Move();
	LightManager::Instance()->SetPointLightPos(useLightNum_, transform_->position_);
	if (hp_ == 0)
	{
		action_ = (uint8_t)EnemyAction::Death;
	}
	if (action_ == (uint8_t)EnemyAction::Attack)
	{
		objectPtr_->GetComponent<Material>()->color_ = { 0,0,0,0 };
	}

	if (hp_ == 1)
	{
		LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.z,sDefaultColor_.y,sDefaultColor_.x });
	}
	else if (hp_ == 2)
	{
		LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.z,sDefaultColor_.x,sDefaultColor_.y });
	}
	else if (hp_ == 3)
	{
		LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.x,sDefaultColor_.x,sDefaultColor_.z });
	}
	else if (hp_ == 4)
	{
		LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.x,sDefaultColor_.z,sDefaultColor_.z });
	}
	else if (hp_ == 5)
	{
		LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.x,sDefaultColor_.z,sDefaultColor_.x });
	}
}

void IFE::Enemy::OnColliderHit(Collider* collider)
{
	if (action_ == (uint8_t)EnemyAction::Death)return;
	if (action_ == (uint8_t)EnemyAction::Hit)return;
	auto bullet = collider->GetObjectPtr()->GetComponent<Bullet>();
	if (hp_ > 0 && bullet)
	{
		sScore_ += hp_;
		hp_--;
		hitTimer_ = 0;
		Vector3 vec = bullet->GetMoveVector();
		hitPos_ = transform_->position_;
		hitAfterPos_ = transform_->position_ + (Float3)vec * 10;
		action_ = (uint8_t)EnemyAction::Hit;
	}
}

void IFE::Enemy::SetPlayerTransform(Transform* transform)
{
	playerTransform_ = transform;
}

uint8_t IFE::Enemy::GetHP()
{
	return hp_;
}

void IFE::Enemy::SetHP(uint8_t hp)
{
	hp_ = hp;
}

void IFE::Enemy::SetLight()
{
	if (useLightNum_ == uint8_t(-1))
	{
		useLightNum_ = LightManager::GetPointLightNumber();
		LightManager::Instance()->SetPointLightAtten(useLightNum_, { 0.0025f,0.015f,0.0025f });
		if (hp_ == 1)
		{
			LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.z,sDefaultColor_.y,sDefaultColor_.x });
		}
		else if (hp_ == 2)
		{
			LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.z,sDefaultColor_.x,sDefaultColor_.y });
		}
		else if (hp_ == 3)
		{
			LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.x,sDefaultColor_.x,sDefaultColor_.z });
		}
		else if (hp_ == 4)
		{
			LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.x,sDefaultColor_.y,sDefaultColor_.z });
		}
		else if (hp_ == 5)
		{
			LightManager::Instance()->SetPointLightColor(useLightNum_, { sDefaultColor_.x,sDefaultColor_.z,sDefaultColor_.x });
		}
		LightManager::Instance()->SetPointLightActive(useLightNum_, true);
		LightManager::Instance()->SetPointLightPos(useLightNum_, transform_->position_);
	}
}

void IFE::Enemy::SetAction(EnemyAction action)
{
	action_ = uint8_t(action);
}

uint8_t IFE::Enemy::GetAction()
{
	return action_;
}

void IFE::Enemy::Move()
{
	Vector3 vec = playerTransform_->position_ - transform_->position_;
	float len = vec.Length();
	if (Player::sMoveFlag_)
	{
		isFoundPlayer_ = len < sDetectionDistance_;
	}
	size_t num = hp_;
	if (hp_ != 0)num--;
	speed_ = sHpSpeed_[num];
	(this->*ActtionTable[action_])();
}

void IFE::Enemy::Stanby()
{
	action_ = patrolPoint_.size() > 1 ? uint8_t(EnemyAction::Patrol) : action_;
}

void IFE::Enemy::Patrol()
{
	if (patrolPoint_.size() < 2)
	{
		action_ = uint8_t(EnemyAction::Stanby);
		return;
	}

	uint8_t nextPoint = nowPoint_ + 1 < patrolPoint_.size() ? nowPoint_ + 1 : 0;

	Float3 nextPointPos = patrolPoint_[nextPoint];

	Vector3 vec = nextPointPos - transform_->position_;
	vec.Normalize();
	transform_->position_ += vec * speed_ * IFETime::sDeltaTime_;

	if (NearEqual(transform_->position_, nextPointPos, speed_ / 2))
	{
		nowPoint_ = nextPoint;
	}
}

void IFE::Enemy::Detection()
{
	Float3 pPos = playerTransform_->position_;
	Float3 ePos = transform_->position_;

	Vector3 vec = pPos - ePos;
	float distance = vec.Length();
	vec.Normalize();
	transform_->position_ += vec * speed_ * IFETime::sDeltaTime_;

	action_ = distance <= 10 ? uint8_t(EnemyAction::Attack) : action_;
	action_ = distance > sDetectionDistance_ ? uint8_t(EnemyAction::Stanby) : action_;
}

void IFE::Enemy::Attack()
{

	//auto e = ParticleManager::Instance()->Instantiate("Chrysanthemum", transform_->position_);
	//e->GetComponent<FireworkChrysanthemum>()->StartFirework();

	//objectPtr_->Destroy();
}

void IFE::Enemy::Death()
{
	deathDirectionTimer_ += IFETime::sDeltaTime_;
	float xz = OutQuad(0.0025f, 1, deathDirectionMaxTime_, deathDirectionTimer_);
	float y = OutQuad(0.015f, 1, deathDirectionMaxTime_, deathDirectionTimer_);
	LightManager::Instance()->SetPointLightAtten(useLightNum_, { xz,y,xz });
	if (deathDirectionTimer_ > deathDirectionMaxTime_)
	{
		LightManager::Instance()->SetPointLightActive(useLightNum_, false);
		objectPtr_->Destroy();
	}
}

void IFE::Enemy::Hit()
{
	hitTimer_ += IFETime::sDeltaTime_;
	//transform_->position_ = OutQuadFloat3(hitPos_, hitAfterPos_, sMaxHitTime_, hitTimer_);
	if (hitTimer_ > sMaxHitTime_)
	{
		action_ = (uint8_t)EnemyAction::Stanby;
	}
	if (patrolPoint_.size() > 0)Patrol();
}

void (Enemy::* Enemy::ActtionTable[])() =
{
	&Enemy::Stanby,&Enemy::Patrol,&Enemy::Detection,&Enemy::Attack,&Enemy::Death,&Enemy::Hit,
};

#ifdef NDEBUG
#else
#include "ImguiManager.h"
void IFE::Enemy::ComponentDebugGUI()
{
	ImguiManager* gui = ImguiManager::Instance();
	if (gui->NewTreeNode("status view"))
	{
		std::string hpText = "HP : " + std::to_string(hp_);
		gui->TextGUI(hpText);
		std::string actionText = "action : ";
		switch (action_)
		{
		case uint8_t(EnemyAction::Stanby):
			actionText += "Stanby";
			break;
		case uint8_t(EnemyAction::Patrol):
			actionText += "Patrol";
			break;
		case uint8_t(EnemyAction::Detection):
			actionText += "Detection";
			break;
		case uint8_t(EnemyAction::Attack):
			actionText += "Attack";
			break;
		case uint8_t(EnemyAction::Death):
			actionText += "Death";
			break;
		}
		gui->TextGUI(actionText);
		gui->EndTreeNode();
	}
	gui->DragFloatGUI(&sHpSpeed_[0], "speed hp 1");
	gui->DragFloatGUI(&sHpSpeed_[1], "speed hp 2");
	gui->DragFloatGUI(&sHpSpeed_[2], "speed hp 3");
	gui->DragFloatGUI(&sHpSpeed_[3], "speed hp 4");
	gui->DragFloatGUI(&sHpSpeed_[4], "speed hp 5");
	gui->DragVectorFloat3GUI(patrolPoint_, "patrol point", transform_->position_);
	if (gui->NewTreeNode("start Action"))
	{
		int32_t tempNum = action_;
		gui->RadioButtonGUI("Stanby", &tempNum, uint32_t(EnemyAction::Stanby));
		gui->RadioButtonGUI("Patrol", &tempNum, uint32_t(EnemyAction::Patrol));
		gui->RadioButtonGUI("Detection", &tempNum, uint32_t(EnemyAction::Detection));
		gui->RadioButtonGUI("Attack", &tempNum, uint32_t(EnemyAction::Attack));
		gui->RadioButtonGUI("Death", &tempNum, uint32_t(EnemyAction::Death));
		action_ = uint8_t(tempNum);
		gui->EndTreeNode();
	}
}

void IFE::Enemy::OutputComponent(nlohmann::json& j)
{
	j["speed"] = speed_;
	j["action"] = action_;
	j["patrolPointSize"] = patrolPoint_.size();
	for (uint32_t i = 0; i < patrolPoint_.size(); i++)
	{
		JsonManager::Instance()->OutputFloat3(j["patrolPoint"][i], patrolPoint_[i]);
	}
	for (uint32_t i = 0; i < 5; i++)
	{
		j["HpSpeed"][i] = sHpSpeed_[i];
	}
}
#endif

void IFE::Enemy::LoadingComponent(nlohmann::json& j)
{
	speed_ = j["speed"];
	action_ = j["action"];
	if (j["patrolPointSize"] != 0)
	{
		for (auto& point : j["patrolPoint"])
		{
			patrolPoint_.push_back(JsonManager::Instance()->InputFloat3(point));
		}
	}
	size_t i = 0;
	for (auto& speed : j["HpSpeed"])
	{
		sHpSpeed_[i] = speed;
		i++;
	}
}
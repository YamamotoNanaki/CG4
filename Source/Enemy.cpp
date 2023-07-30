#include "Enemy.h"
#include "IFETime.h"
#include "Transform.h"
#include "Collider.h"
#include "Bullet.h"
#include "Object3D.h"
#include "Material.h"
#include "TransferGeometryBuffer.h"
#include "Ease.h"

using namespace IFE;

Transform* Enemy::playerTransform_ = nullptr;

void IFE::Enemy::Initialize()
{
	if (transform_ != nullptr && action_ == (uint8_t)EnemyAction::Patrol)
	{
		transform_->position_ = patrolPoint_[0];
	}
}

void IFE::Enemy::Update()
{
	Move();
	if (hp_ == 2)
	{
		objectPtr_->GetComponent<Material>()->color_ = { 1,0,0,1 };
	}
	else if (hp_ == 1)
	{
		objectPtr_->GetComponent<Material>()->color_ = { 0,1,0,1 };
	}
	else
	{
		objectPtr_->GetComponent<Material>()->color_ = { 0,0,1,1 };
		action_ = (uint8_t)EnemyAction::Death;
	}
	//if (isHit_)
	//{
	//	objectPtr_->GetComponent<Material>()->color_ = { 1,0,0,1 };
	//	colorTimer_ += IFETime::sDeltaTime_;
	//	if (colorTimer_ > 0.7f)
	//	{
	//		isHit_ = false;
	//	}
	//}
	//else objectPtr_->GetComponent<Material>()->color_ = { 0,0,1,1 };
}

void IFE::Enemy::OnColliderHit(Collider* collider)
{
	if (collider->GetObjectPtr()->GetComponent<Bullet>())
	{
		//colorTimer_ = 0;
		//isHit_ = true;
		hp_--;
	}
}

void IFE::Enemy::SetPlayerTransform(Transform* transform)
{
	playerTransform_ = transform;
}

void IFE::Enemy::Move()
{
	Vector3 vec = playerTransform_->position_ - transform_->position_;
	float len = vec.Length();
	isFoundPlayer_ = len < sDetectionDistance_;
	(this->*ActtionTable[action_])();
}

void IFE::Enemy::Stanby()
{
	action_ = patrolPoint_.size() > 1 ? uint8_t(EnemyAction::Patrol) : action_;
	action_ = isFoundPlayer_ == true ? uint8_t(EnemyAction::Detection) : action_;
}

void IFE::Enemy::Patrol()
{
	if (patrolPoint_.size() < 2)
	{
		action_ = uint8_t(EnemyAction::Stanby);
		return;
	}

	uint8_t nextPoint = nowPoint_ + 1 < patrolPoint_.size() ? nowPoint_ + 1 : 0;

	Float3 nowPointPos = patrolPoint_[nowPoint_];
	Float3 nextPointPos = patrolPoint_[nextPoint];

	Vector3 vec = nextPointPos - nowPointPos;
	vec.Normalize();
	transform_->position_ += vec * speed_ * IFETime::sDeltaTime_;

	if (NearEqual(transform_->position_, nextPointPos, 0.25))
	{
		nowPoint_ = nextPoint;
	}

	action_ = isFoundPlayer_ == true ? uint8_t(EnemyAction::Detection) : action_;
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
	attackDirectionTimer_ += IFETime::sDeltaTime_;
	if (attackDirectionTimer_ > attackDirectionMaxTime_)
	{
		objectPtr_->Destroy();
	}
}

void IFE::Enemy::Death()
{
	deathDirectionTimer_ += IFETime::sDeltaTime_;
	auto tgb = objectPtr_->GetComponent<TransferGeometryBuffer>();
	tgb->explosion_ = deathDirectionTimer_ * 150;
	tgb->rotation_ = deathDirectionTimer_ * 25;
	tgb->gravity_ = deathDirectionTimer_ * 35;
	tgb->polygonSize_ = Lerp(5, 0, deathDirectionMaxTime_, deathDirectionTimer_);
	objectPtr_->GetComponent<Material>()->color_.z = Lerp(1, 0, deathDirectionMaxTime_, deathDirectionTimer_);
	if (deathDirectionTimer_ > deathDirectionMaxTime_)
	{
		objectPtr_->Destroy();
	}
}

void (Enemy::* Enemy::ActtionTable[])() =
{
	&Enemy::Stanby,&Enemy::Patrol,&Enemy::Detection,&Enemy::Attack,&Enemy::Death,
};

#ifdef _DEBUG
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
	gui->DragFloatGUI(&speed_, "speed");
	gui->DragVectorFloat3GUI(patrolPoint_, "patrol point");
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
}
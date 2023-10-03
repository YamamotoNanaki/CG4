#include "Boss.h"
#include "Object3D.h"
#include "Collider.h"
#include "Transform.h"
#include "IFETime.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "ObjectManager.h"
#include "BulletParticle.h"
#include "Emitter.h"

using namespace IFE;

void IFE::Boss::Initialize()
{
	if (transform_)pos_ = transform_->position_;
	pm_ = ParticleManager::Instance();
	playerPos_ = &ObjectManager::Instance()->GetObjectPtr("Player")->transform_->position_;
}

void IFE::Boss::Update()
{
	Move();
}

void IFE::Boss::Move()
{
	Gravity();
	transform_->position_ = pos_;
	(this->*ActtionTable[action_])();
}

void (Boss::* Boss::ActtionTable[])() =
{
	&Boss::Start,&Boss::Stanby,&Boss::Attack1,&Boss::Attack2,&Boss::Attack3,&Boss::Death
};

void IFE::Boss::Gravity()
{
	if (objectPtr_->GetComponent<Collider>()->onGround_)
	{
		gravity_ = 0;
		return;
	}
	gravity_ += 4.9f * IFETime::sDeltaTime_;
	pos_.y -= gravity_;
}

void IFE::Boss::Start()
{
	if (Player::sMoveFlag_)
	{
		action_ = (uint8_t)BossAction::Stanby;
	}
}

void IFE::Boss::Stanby()
{
	actionTimer_ += IFETime::sDeltaTime_;
	if (actionTimer_ > 3)
	{
		actionTimer_ = 0;
		bulletNum_ = 0;
		action_ = (uint8_t)BossAction::Attack1;
	}
}

void IFE::Boss::Attack1()
{
	actionTimer_ += IFETime::sDeltaTime_;
	if (actionTimer_ > 1)
	{
		actionTimer_ = 0;
		bulletNum_++;
		Float3 pos = { pos_.x,pos_.y + transform_->scale_.y,pos_.z};
		auto bullet = ObjectManager::Instance()->Instantiate("EnemyBullet", pos);
		if (bullet == nullptr)return;
		Vector3 vec;
		vec = *playerPos_ - pos;
		vec.Normalize();

		bullet->GetComponent<EnemyBullet>()->SetMoveVector(vec);
		auto emitter = pm_->Instantiate("Bullet");
		if (emitter)
		{
			emitter->GetComponent<BulletParticle>()->GetBullet(bullet);
			emitter->isActive_ = true;
		}

		//Sound::Instance()->SoundPlay("shot", false);
	}
	if (bulletNum_ >= 10)
	{
		action_ = (uint8_t)BossAction::Stanby;
	}
}

void IFE::Boss::Attack2()
{
}

void IFE::Boss::Attack3()
{
}

void IFE::Boss::Death()
{
}

#ifdef NDEBUG
#else
#include "ImguiManager.h"
void IFE::Boss::ComponentDebugGUI()
{
	ImguiManager* gui = ImguiManager::Instance();
	if (gui->NewTreeNode("status view"))
	{
		std::string hpText = "HP : " + std::to_string(hp_);
		gui->TextGUI(hpText);
		std::string actionText = "action : ";
		switch (action_)
		{
		case uint8_t(BossAction::Start):
			actionText += "Start";
			break;
		case uint8_t(BossAction::Stanby):
			actionText += "Stanby";
			break;
		case uint8_t(BossAction::Attack1):
			actionText += "Attack1";
			break;
		case uint8_t(BossAction::Attack2):
			actionText += "Attack2";
			break;
		case uint8_t(BossAction::Attack3):
			actionText += "Attack3";
			break;
		case uint8_t(BossAction::Death):
			actionText += "Death";
			break;
		}
		gui->TextGUI(actionText);
		gui->EndTreeNode();
	}
}
#endif
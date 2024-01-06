#include "LaunchFirework.h"
#include "Transform.h"
#include "IFETime.h"
#include "Rand.h"
#include "Object3D.h"
#include "ParticleManager.h"
#include "FireworkChrysanthemum.h"
#include "Sound.h"
#include "BulletParticle.h"
#include "Ease.h"

void LaunchFirework::Initialize()
{
	targetY_ = IFE::IFERand::GetRandF(25, 45);
}

void LaunchFirework::Update()
{
	if (phase_ == uint8_t(LaunchAction::Move))
	{
		timer_ += IFE::IFETime::sDeltaTime_;
		if (timer_ > maxTime_)
		{
			speed_ = 25;
		}
		else
		{
			speed_ = IFE::InQuad(35, 25, maxTime_, timer_);
		}
		transform_->position_.y += speed_ * IFE::IFETime::sDeltaTime_;
		IFE::Float2 r = { IFE::IFERand::GetRandF(-2,2),IFE::IFERand::GetRandF(-5,5) };
		transform_->position_.x += r.x * IFE::IFETime::sDeltaTime_;
		transform_->position_.z += r.y * IFE::IFETime::sDeltaTime_;
		if (transform_->position_.y > targetY_)
		{
			phase_++;
		}
	}
	else
	{
		for (size_t i = 0; i < 3; i++)
		{
			auto e = IFE::ParticleManager::Instance()->Instantiate("Chrysanthemum", transform_->position_);
			e->GetComponent<IFE::FireworkChrysanthemum>()->StartFirework(i);
			e->GetComponent<IFE::FireworkChrysanthemum>()->SetColor(bulletEmitter_->GetComponent<IFE::BulletParticle>()->GetColor());
		}
		IFE::Sound::Instance()->SoundPlay("Firework", false);
		objectPtr_->Destroy();
	}
}

void LaunchFirework::SetEmitter(IFE::Emitter* emitter)
{
	bulletEmitter_ = emitter;
}

#include "TutorialMove.h"
#include "Collider.h"
#include "Player.h"
#include "Object3D.h"
#include "SpriteManager.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "Transform.h"

void TutorialMove::Initialize()
{
	LStick_ = IFE::SpriteManager::Instance()->GetSpritePtr("AnalogL");
	RStick_ = IFE::SpriteManager::Instance()->GetSpritePtr("AnalogR");
	idou_ = IFE::SpriteManager::Instance()->GetSpritePtr("idou");
	camera_ = IFE::SpriteManager::Instance()->GetSpritePtr("kamera");
	tutorialEnd_ = false;
}

void TutorialMove::Update()
{
	if (!tutorialEnd_)
	{
		if (stayPlayer_)
		{
			stayPlayer_ = false;
		}
		else
		{
			displayEnd_ = true;
		}
		if (displayEnd_)
		{
			timer_ += IFE::IFETime::sDeltaTime_;
			float a = IFE::InOutQuad(1, 0, maxTimer_, timer_);
			IFE::Float2 l = IFE::InOutQuad(IFE::Float2(534, 450), IFE::Float2(1250, 795), maxTimer_, timer_);
			IFE::Float2 r = IFE::InOutQuad(IFE::Float2(1066, 450), IFE::Float2(1500, 795), maxTimer_, timer_);
			idou_->GetComponent<IFE::ColorBuffer>()->SetAlpha(a);
			camera_->GetComponent<IFE::ColorBuffer>()->SetAlpha(a);
			LStick_->transform_->position2D_ = l;
			RStick_->transform_->position2D_ = r;
			if (timer_ >= maxTimer_)
			{
				tutorialEnd_ = true;
				idou_->GetComponent<IFE::ColorBuffer>()->SetAlpha(0);
				camera_->GetComponent<IFE::ColorBuffer>()->SetAlpha(0);
				LStick_->transform_->position2D_ = IFE::Float2(1250, 795);
				RStick_->transform_->position2D_ = IFE::Float2(1500, 795);
			}
		}
	}
}

void TutorialMove::OnColliderHit(IFE::Collider* collider)
{
	if (!displayEnd_ && collider->GetObjectPtr()->GetComponent<IFE::Player>())
	{
		stayPlayer_ = true;
	}
}

#include "TutorialAttack.h"
#include "TutorialMove.h"
#include "Collider.h"
#include "Player.h"
#include "Object3D.h"
#include "SpriteManager.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "Transform.h"
#include "ObjectManager.h"
#include "Enemy.h"

void TutorialAttack::Initialize()
{
	RButton_ = IFE::SpriteManager::Instance()->GetSpritePtr("ButtonR");
	attack_ = IFE::SpriteManager::Instance()->GetSpritePtr("kougeki");
	tutorialEnd_ = false;
}

void TutorialAttack::Update()
{
	if (!tutorialEnd_)
	{
		if (!tutorialStart_)
		{
			if (stayPlayer_)
			{
				timer_ += IFE::IFETime::sDeltaTime_;
				float a = IFE::InOutQuad(0, 1, maxTimer_, timer_);
				attack_->GetComponent<IFE::ColorBuffer>()->SetAlpha(a);
				RButton_->GetComponent<IFE::ColorBuffer>()->SetAlpha(a);
				if (timer_ >= maxTimer_)
				{
					tutorialStart_ = true;
					timer_ = 0;
					attack_->GetComponent<IFE::ColorBuffer>()->SetAlpha(1);
					RButton_->GetComponent<IFE::ColorBuffer>()->SetAlpha(1);
				}
			}
		}
		else
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
				IFE::Float2 r = IFE::InOutQuad(IFE::Float2(1066, 450), IFE::Float2(1485, 660), maxTimer_, timer_);
				attack_->GetComponent<IFE::ColorBuffer>()->SetAlpha(a);
				RButton_->transform_->position2D_ = r;
				if (timer_ >= maxTimer_)
				{
					tutorialEnd_ = true;
					attack_->GetComponent<IFE::ColorBuffer>()->SetAlpha(0);
					RButton_->transform_->position2D_ = IFE::Float2(1485, 660);
				}
			}
		}
	}
}

void TutorialAttack::OnColliderHit(IFE::Collider* collider)
{
	if (!displayEnd_ && TutorialMove::tutorialEnd_ && collider->GetObjectPtr()->GetComponent<IFE::Player>())
	{
		stayPlayer_ = true;
	}
}

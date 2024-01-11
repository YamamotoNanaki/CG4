#include "FireworkTrail.h"
#include "Emitter.h"
#include "Transform.h"
#include "ColorBuffer.h"

using namespace IFE;

void FireworkTrail::Initialize()
{
	pattern_ = next_;
	next_++;
	if (next_ >= 2)next_ -= 2;
	for (size_t i = 0; i < 400; i++)
	{
		auto p = emitterPtr_->AddParticle({ 0,0,0 });
		p->AddComponent<ColorBuffer>();
	}
	emitterPtr_->timerFlag_ = false;
}

void FireworkTrail::Update()
{
	timer_ += IFETime::sDeltaTime_;
	endTimer_ += IFETime::sDeltaTime_;
	if (endTimer_ > endTime_)
	{
		emitterPtr_->deleteFlag_ = true;
	}
	if (/*timer_ > maxTime_*/true)
	{
		timer_ = 0;
		int16_t j = 0;
		if (pattern_ == 0.1f)
		{
			j = 50;
		}
		std::list<std::unique_ptr<Particle>>::iterator itrA;
		std::list<std::unique_ptr<Particle>>::iterator itrB;
		int16_t i = 0;
		for (; itrA != emitterPtr_->particles_.end(); ++itrA)
		{
			itrB = itrA;
			++itrB;
			auto color = itrA->get()->GetComponent<ColorBuffer>();
			auto transform = itrA->get()->transform_;
			if (i % 8 == 7)
			{
				int16_t k = 0;
				for (auto& parent : parentPtr_->particles_)
				{
					if (j == k)
					{
						transform->position_ = parent->transform_->position_;
						transform->scale_ = parent->transform_->scale_ * 0.7f;
						color->SetColor(parent->GetComponent<ColorBuffer>()->GetColor());
						break;
					}
					k++;
				}
			}
			else
			{
				float offset = 0.7f;
				transform->position_ = itrB->get()->transform_->position_;
				transform->scale_ = itrB->get()->transform_->scale_ * offset;
				Float4 col = itrB->get()->GetComponent<ColorBuffer>()->GetColor();
				col *= offset;
				color->SetColor(col);
			}
		}
	}
}

void FireworkTrail::SetParent(IFE::Emitter* emitter)
{
	parentPtr_ = emitter;
}

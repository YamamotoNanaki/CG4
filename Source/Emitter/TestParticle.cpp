#include "TestParticle.h"
#include "Emitter.h"
#include "Transform.h"


void IFE::TestParticle::Update()
{
	for (auto& itr : emitterPtr_->particles_)
	{
		itr->transform_->position_.x++;
	}
	emitterPtr_->AddParticle();
}

#pragma once
#include "Compute.h"

namespace IFE
{
	struct Particle
	{
		Float3 position;
		float speed;
		Float3 velocity;
		float lifeTime;
	};
	class Emitter
	{
		std::vector<Particle>particles;
		Compute<Particle> compute;
	};
}

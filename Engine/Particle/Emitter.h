#pragma once
#include "Compute.h"

namespace IFE
{
	struct Particle
	{
		Float3 position;
		float speed;
		Float3 velocity;
		float time;
	};
	class Emitter
	{
	private:
		std::vector<Particle>particles;
		Compute<Particle> compute;

	public:
		void Initialize();
		void Update();
		void Draw();

	private:
	};
}

#pragma once
#include "Component.h"
#include "Vector3.h"

namespace IFE
{
	class Player : public Component
	{
	private:
		float speed_ = 0.3f;
		Vector3 move_;
	public:
		void Update()override;
		void OnColliderHit(Collider* collider)override;

	private:
		void Move();
	};
}

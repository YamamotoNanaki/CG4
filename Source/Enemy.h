#pragma once
#include <Component.h>

namespace IFE
{
	class Enemy : public Component
	{
	private:
		bool left_ = false;
		float speed_ = 0.1f;
		bool isHit_ = false;
		float colorTimer_ = 0;
	public:
		void Update()override;
		void OnColliderHit(Collider* collider)override;

	private:
		void Move();
	};
}

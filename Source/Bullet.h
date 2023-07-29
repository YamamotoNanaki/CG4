#pragma once
#include "Component.h"

namespace IFE
{
	class Bullet : public Component
	{
	private:
		float speed_ = 200;
		Vector2 moveVec_;
		float lifeTimer_ = 0;
		float timerMax_ = 5;
		inline static Emitter* sChrysanthemum_ = nullptr;
	public:
		void Update()override;
		void OnColliderHit(Collider* collider)override;
		void SetMoveVector(const Vector2& moveVec);

	private:
		void Move();
		void Timer();
	};
}

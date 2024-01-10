#pragma once
#include "Component.h"

namespace IFE
{
	class Bullet : public Component
	{
	private:
		float speed_ = 200;
		Vector3 moveVec_;
		float lifeTimer_ = 0;
		float timerMax_ = 5;
		inline static Emitter* sChrysanthemum_ = nullptr;
		Emitter* bulletEmitter_;
	public:
		void Update()override;
		void OnColliderHit(Collider* collider)override;
		void SetMoveVector(const Vector3& moveVec);
		Vector3 GetMoveVector();
		void SetBulletEmitter(Emitter* emitter);

	private:
		void Move();
		void Timer();
	};
}

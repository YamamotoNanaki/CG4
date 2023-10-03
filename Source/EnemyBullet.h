#pragma once
#include "Component.h"

namespace IFE
{
	class EnemyBullet : public Component
	{
	private:
		float speed_ = 170;
		Vector3 moveVec_;
		float lifeTimer_ = 0;
		float timerMax_ = 5;
		Emitter* bulletEmitter_;
	public:
		void Update()override;
		void OnColliderHit(Collider* collider)override;
		void SetMoveVector(const Vector3& moveVec);
		void SetBulletEmitter(Emitter* emitter);
		Vector3 GetMoveVector();

	private:
		void Move();
		void Timer();
	};
}

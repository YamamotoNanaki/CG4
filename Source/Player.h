#pragma once
#include "Component.h"
#include "Vector3.h"
#include "ParticleManager.h"

namespace IFE
{
	class Player : public Component
	{
	private:
		float speed_ = 10;
		Vector3 move_;
		Float3 oldPos_;
		Float3 pos_;
		Vector2 moveVec_;
		float nextBulletTime_ = 0.3f;
		float gravity_ = 1;
		bool soundFlag_ = false;
		inline static ParticleManager* pm_ = nullptr;
		bool invincible_ = false;
		float invincibleTimer_ = 0;
		float invincibleMaxTime = 3;

		float deathTimer_ = 0;
		float maxDeathTime_ = 3;
	public:
		int16_t hp_ = 5;
		void Initialize()override;
		void Update()override;
		void OnColliderHit(Collider* collider)override;

#ifdef NDEBUG
#else
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;

	private:
		void Move();
		void Rota();
		void CameraFollow();
		void EnemyCollide();
		void Shoot();
		void Death();
	};
}

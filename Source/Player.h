#pragma once
#include "Component.h"
#include "Vector3.h"
#include "ParticleManager.h"

namespace IFE
{
	class Camera;
	class Player : public Component
	{
	private:
		float speed_ = 10;
		Vector3 move_;
		Vector2 moveVec_;
		float nextBulletTime_ = 0.3f;
		float gravity_ = 1;
		bool soundFlag_ = false;
		inline static ParticleManager* pm_ = nullptr;
		bool invincible_ = false;
		float invincibleTimer_ = 0;
		float invincibleMaxTime = 3;
		Camera* playerCamera_ = nullptr;
		float deathTimer_ = 0;
		float maxDeathTime_ = 3;
	public:
		Float3 oldPos_;
		Float3 pos_;
		inline static bool sMoveFlag_ = false;
		int16_t hp_ = 5;
	public:
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
		//void Start();
		void Rota();
		void EnemyCollide();
		void Shoot();
		void Death();
		void Gravity();
	};
}

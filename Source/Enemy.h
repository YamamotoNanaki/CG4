#pragma once
#include <Component.h>
#include "JsonManager.h"

namespace IFE
{
	enum class EnemyAction
	{
		Stanby, Patrol, Detection, Attack, Death
	};
	class Enemy : public Component
	{
	private:
		float speed_ = 1;
		uint8_t action_;
		bool isFoundPlayer_ = false;

		std::vector<Float3>patrolPoint_;
		uint8_t nowPoint_ = 0;

		uint8_t hp_ = 2;

		static Transform* playerTransform_;
		float deathDirectionTimer_;
		float deathDirectionMaxTime_ = 0.5;

	public:
		void Initialize()override;
		void Update()override;
		void OnColliderHit(Collider* collider)override;

		static void SetPlayerTransform(Transform* transform);

	private:
		void Move();
		//停止または待機
		//待機も―チョンを動かし座標移動はしない
		void Stanby();
		//巡回
		void Patrol();
		//プレイヤー発見時
		void Detection();
		//プレイヤー攻撃時
		void Attack();
		//死亡時
		void Death();
		static void (Enemy::* ActtionTable[])();


#ifdef _DEBUG
	public:
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& j)override;

	private:
#endif

		void LoadingComponent(nlohmann::json& j)override;
	};
}

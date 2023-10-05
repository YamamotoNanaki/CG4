#pragma once
#include <Component.h>
#include "JsonManager.h"


namespace IFE
{
	enum class EnemyAction
	{
		Stanby, Patrol, Detection, Attack, Death, Hit
	};
	class Enemy : public Component
	{
	public:
		inline static uint32_t sDeathEnemyNum_ = 0;
	private:
		//1秒に進むスピード
		float speed_ = 1;
		//現在のアクション
		uint8_t action_;
		//プレイヤーを見つけたかどうか
		bool isFoundPlayer_ = false;

		//巡回地点
		std::vector<Float3>patrolPoint_;
		uint8_t nowPoint_ = 0;

		//敵のHP
		uint8_t hp_ = 2;
		bool attackFlag_ = false;
		float hitTimer_ = 0;
		inline static float sMaxHitTime_ = 0.5f;
		Float3 hitAfterPos_;
		Float3 hitPos_;

		//プレイヤーのトランスフォーム参照用
		static Transform* playerTransform_;
		//死亡演出用の時間
		float deathDirectionTimer_;
		float deathDirectionMaxTime_ = 0.5;
		//攻撃演出用の時間
		float attackDirectionTimer_;
		float attackDirectionMaxTime_ = 0.5;
		//発見距離
		inline const static float sDetectionDistance_ = 80;

	public:
		void Initialize()override;
		void Update()override;
		void OnColliderHit(Collider* collider)override;

		//プレイヤーのトランスフォーム参照用
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
		//攻撃を受けた時
		void Hit();
		static void (Enemy::* ActtionTable[])();


#ifdef NDEBUG
#else
	public:
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& j)override;

	private:
#endif

		void LoadingComponent(nlohmann::json& j)override;
	};
}

#pragma once
#include <Component.h>
#include "JsonManager.h"

namespace IFE
{
	enum class EnemyAction
	{
		Stanby, Patrol, Detection, Attack, Death, Hit
	};
	enum class EnemyColor
	{
		Blue, Green, Yellow, Red, Purple
	};
	class Enemy : public Component
	{
	public:
		inline static uint32_t sScore_ = 0;
	private:
		inline static IFE::Float3 sDefaultColor_ = { 2.55f,1.05f,0.25f };
		inline static float sHpSpeed_[5];
		//1秒に進むスピード
		float speed_ = 1;
		//現在のアクション
		uint8_t action_;
		//プレイヤーを見つけたかどうか
		bool isFoundPlayer_ = false;

		//使用ライトナンバー
		uint8_t useLightNum_ = (uint8_t)-1;

		//巡回地点
		std::vector<Float3>patrolPoint_;
		uint8_t nowPoint_ = 0;

		//敵のHP
		uint8_t hp_ = 1;
		bool attackFlag_ = false;
		float hitTimer_ = 0;
		inline static float sMaxHitTime_ = 0.5;
		Float3 hitAfterPos_;
		Float3 hitPos_;

		//プレイヤーのトランスフォーム参照用
		static Transform* playerTransform_;
		//死亡演出用の時間
		float deathDirectionTimer_;
		float deathDirectionMaxTime_ = 5;
		//攻撃演出用の時間
		float attackDirectionTimer_;
		float attackDirectionMaxTime_ = 0.5;
		//発見距離
		inline const static float sDetectionDistance_ = 80;

		bool visual_ = false;

	public:
		void Initialize()override;
		void Update()override;
		void OnColliderHit(Collider* collider)override;

		//プレイヤーのトランスフォーム参照用
		static void SetPlayerTransform(Transform* transform);
		uint8_t GetHP();
		void SetHP(uint8_t hp);
		void SetLight();
		void SetAction(EnemyAction action);
		uint8_t GetAction();
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

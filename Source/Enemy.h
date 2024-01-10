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
		//1�b�ɐi�ރX�s�[�h
		float speed_ = 1;
		//���݂̃A�N�V����
		uint8_t action_;
		//�v���C���[�����������ǂ���
		bool isFoundPlayer_ = false;

		//�g�p���C�g�i���o�[
		uint8_t useLightNum_ = (uint8_t)-1;

		//����n�_
		std::vector<Float3>patrolPoint_;
		uint8_t nowPoint_ = 0;

		//�G��HP
		uint8_t hp_ = 1;
		bool attackFlag_ = false;
		float hitTimer_ = 0;
		inline static float sMaxHitTime_ = 0.5;
		Float3 hitAfterPos_;
		Float3 hitPos_;

		//�v���C���[�̃g�����X�t�H�[���Q�Ɨp
		static Transform* playerTransform_;
		//���S���o�p�̎���
		float deathDirectionTimer_;
		float deathDirectionMaxTime_ = 5;
		//�U�����o�p�̎���
		float attackDirectionTimer_;
		float attackDirectionMaxTime_ = 0.5;
		//��������
		inline const static float sDetectionDistance_ = 80;

		bool visual_ = false;

	public:
		void Initialize()override;
		void Update()override;
		void OnColliderHit(Collider* collider)override;

		//�v���C���[�̃g�����X�t�H�[���Q�Ɨp
		static void SetPlayerTransform(Transform* transform);
		uint8_t GetHP();
		void SetHP(uint8_t hp);
		void SetLight();
		void SetAction(EnemyAction action);
		uint8_t GetAction();
	private:
		void Move();
		//��~�܂��͑ҋ@
		//�ҋ@���\�`�����𓮂������W�ړ��͂��Ȃ�
		void Stanby();
		//����
		void Patrol();
		//�v���C���[������
		void Detection();
		//�v���C���[�U����
		void Attack();
		//���S��
		void Death();
		//�U�����󂯂���
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

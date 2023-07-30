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
		//1�b�ɐi�ރX�s�[�h
		float speed_ = 1;
		//���݂̃A�N�V����
		uint8_t action_;
		//�v���C���[�����������ǂ���
		bool isFoundPlayer_ = false;

		//����n�_
		std::vector<Float3>patrolPoint_;
		uint8_t nowPoint_ = 0;

		//�G��HP
		uint8_t hp_ = 2;

		inline static uint32_t sDeathEnemyNum_ = 0;

		//�v���C���[�̃g�����X�t�H�[���Q�Ɨp
		static Transform* playerTransform_;
		//���S���o�p�̎���
		float deathDirectionTimer_;
		float deathDirectionMaxTime_ = 0.5;
		//�U�����o�p�̎���
		float attackDirectionTimer_;
		float attackDirectionMaxTime_ = 0.5;
		//��������
		inline const static float sDetectionDistance_ = 80;

	public:
		void Initialize()override;
		void Update()override;
		void OnColliderHit(Collider* collider)override;

		//�v���C���[�̃g�����X�t�H�[���Q�Ɨp
		static void SetPlayerTransform(Transform* transform);

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

#pragma once
#include "Component.h"
#include "ParticleManager.h"

namespace IFE
{
	enum class BossAction
	{
		Start, Stanby, Attack1, Attack2, Attack3, Death
	};
	class Boss : public Component
	{
	private:
		float gravity_ = 0;
		Float3 pos_;
		const Float3* playerPos_;
		uint8_t action_ = uint8_t(BossAction::Start);
		float actionTimer_ = 0;
		uint32_t hp_ = 0;
		uint8_t bulletNum_ = 0;
		inline static ParticleManager* pm_ = nullptr;
	public:
		void Initialize()override;
		void Update()override;
	private:
		void Move();
		void Gravity();
		void Start();
		void Stanby();
		void Attack1();
		void Attack2();
		void Attack3();
		void Death();
		static void (Boss::* ActtionTable[])();

	public:
#ifdef NDEBUG
#else
		void ComponentDebugGUI();
#endif
	};
}

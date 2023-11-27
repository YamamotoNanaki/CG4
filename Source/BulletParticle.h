#pragma once
#include "Component.h"
#include "ParticleSetting.h"

namespace IFE
{
	class Object3D;
	class BulletParticle : public Component
	{
		inline static IFE::Float3 sDefaultColor_ = { 1,0.5f,0.25f };
		Float3* bulletPos_;
		Object3D* bullet_;
		uint8_t pointLightNum_ = 0;
		uint8_t colorSetting_ = (uint8_t)ParticleColorSetting::Red;

	public:
		void Update()override;
		void SetBullet(Object3D* bullet);
		void SetColor(ParticleColorSetting colorSetting);
		void SetColor(uint8_t colorSetting);
		uint8_t GetColor();
		~BulletParticle();

	private:
		void AddBulletParticle();
		void BlueParticle();
		void GreenParticle();
		void YellowParticle();
		void RedParticle();
		void PurpleParticle();
		static void (BulletParticle::* ColorTableFunc[])();
	};
}

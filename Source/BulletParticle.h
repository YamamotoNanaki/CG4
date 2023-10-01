#pragma once
#include "Component.h"

namespace IFE
{
	class Object3D;
	class BulletParticle : public Component
	{
		Float3* bulletPos_;
		Object3D* bullet_;
		uint8_t num = 0;

	public:
		void Update()override;
		void GetBullet(Object3D* bullet);
		~BulletParticle();
	};
}

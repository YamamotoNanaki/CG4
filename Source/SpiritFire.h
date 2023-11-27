#pragma once
#include "Component.h"

class SpiritFire : public IFE::Component
{
private:
	inline static IFE::Float3 sDefaultColor_ = {0.6f,0.3f,0.1f};
	IFE::Float3* enemyPos_;
	IFE::Object3D* enemy_;
public:
	void Update()override;
	void SetEnemy(IFE::Object3D* enemy);

private:
	void BlueParticle();
	void GreenParticle();
	void YellowParticle();
	void RedParticle();
	void PurpleParticle();
	static void (SpiritFire::* ColorTableFunc[])();
};


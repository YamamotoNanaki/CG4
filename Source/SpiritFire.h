#pragma once
#include "Component.h"

class SpiritFire : public IFE::Component
{
	IFE::Float3* enemyPos_;
	IFE::Object3D* enemy_;
public:
	void Update()override;
	void SetEnemy(IFE::Object3D* enemy);
};


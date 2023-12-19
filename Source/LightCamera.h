#pragma once
#include "Component.h"

class LightCamera : public IFE::Component
{
	IFE::Float3* playerPos_;
public:
	void Initialize()override;
	void Update()override;
};


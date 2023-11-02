#pragma once
#include "Component.h"

class PlayerLantern : public IFE::Component
{
private:
	uint8_t useLightNum_ = 0;
public:
	void Initialize();
	void Draw();
};


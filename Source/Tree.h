#pragma once
#include "Component.h"

class Tree : public IFE::Component
{
	using Component::Component;
public:
	void Initialize()override;
};


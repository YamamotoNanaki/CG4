#pragma once
#include "Component.h"

namespace IFE
{
	class ShadowMapCamera : public Component
	{
	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
	};
}

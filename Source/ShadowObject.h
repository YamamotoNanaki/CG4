#pragma once
#include "Component.h"
#include "ConstBuffer.h"
#include "ConstStruct.h"

namespace IFE
{
	class ShadowObject : public Component
	{
		std::unique_ptr<ConstBuffer<LightVP>> materialBuffer_;
		bool drawFlag_ = false;
		LightVP* constMapMaterial_ = nullptr;
	public:
		void Initialize();
		void Draw();
		void SetDrawFlag(bool flag);
	};
}

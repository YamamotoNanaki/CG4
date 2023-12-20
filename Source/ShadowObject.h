#pragma once
#include "Component.h"
#include "ConstBuffer.h"
#include "ConstStruct.h"

namespace IFE
{
	class ShadowObject : public Component
	{
		std::unique_ptr<ConstBuffer<ConstBufferLightVP>> lightVPBuffer_;
		bool drawFlag_ = false;
		ConstBufferLightVP* constMapLightVP_ = nullptr;
	public:
		void Initialize();
		void Draw();
		void SetDrawFlag(bool flag);
	};
}

#pragma once
#include "Component.h"
#include "ConstStruct.h"
#include "ConstBuffer.h"
#include <memory>

namespace IFE
{
	class ColorBuffer : public Component
	{
		using Component::Component;
		std::unique_ptr<ConstBuffer<ConstBufferColor>> colorBuffer_;
		ConstBufferColor* constMapColor_ = nullptr;
		Float4 color_ = {1,1,1,1};

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
#ifdef _DEBUG
		void DebugGUI();
		void OutputComponent();
#endif
		void LoadingComponent();
	};
}

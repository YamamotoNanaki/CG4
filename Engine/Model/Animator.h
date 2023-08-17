#pragma once
#include "Component.h"
#include "FBXModel.h"
#include "ConstStruct.h"
#include "ConstBuffer.h"

namespace IFE
{
	class Animator : public Component
	{
		uint8_t oldAnimNum_ = 0;
		FBXModel* model_;
		float animTimer_ = 0;
		std::unique_ptr<ConstBuffer<ConstBufferDataSkin>> skinBuffer_;
		ConstBufferDataSkin* constMapSkin_ = nullptr;
	public:
		float animSpeed_ = 1.f;
		uint8_t animNum_ = 0;

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		~Animator();

#ifdef NDEBUG
#else
		void ComponentDebugGUI()override;
#endif
	};
}

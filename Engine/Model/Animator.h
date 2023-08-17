#pragma once
#include "Component.h"
#include "FBXModel.h"
#include "ConstStruct.h"
#include "ConstBuffer.h"

namespace IFE
{
	class Animator : public Component
	{
		FBXModel* model_;
		float animTimer_ = 0;
		std::unique_ptr<ConstBuffer<ConstBufferDataSkin>> skinBuffer_;
		ConstBufferDataSkin* constMapSkin_ = nullptr;
	public:
		float animSpeed_ = 1.f;

	public:
		void Initialize()override;
		void Draw()override;
		~Animator();
	};
}

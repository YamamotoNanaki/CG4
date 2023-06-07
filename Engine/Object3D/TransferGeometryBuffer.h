#pragma once
#include "Component.h"
#include "ConstBuffer.h"
#include "ConstStruct.h"
#include <memory>

namespace IFE
{
	class TransferGeometryBuffer : public Component
	{
		using Component::Component;
		std::unique_ptr<ConstBuffer<ConstBufferModelGeometry>> geometryBuffer;
		ConstBufferModelGeometry* constMapGeometry = nullptr;
		float explosion = 0;
		float gravity = 0;
		float rotation = 0;
		float polygonSize = 1;
		bool lightFlag = true;
		bool toonFlag = false;

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void Copy(Component* component)override;
#ifdef _DEBUG
		void DebugGUI();
		void OutputComponent();
#endif
		void LoadingComponent();
	};
}

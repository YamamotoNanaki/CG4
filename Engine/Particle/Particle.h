#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include "ConstStruct.h"
#include "IFMath.h"
#include "ComponentManager.h"
#include "ConstBuffer.h"
#include "VertexBuffer.h"
#include "Emitter.h"
#include "IFETime.h"

#pragma comment(lib,"d3d12.lib")

namespace IFE
{
	class TransformParticle;
	class Particle : public ComponentManager
	{
	public:
		static VertexBuffer<VertexPos>vb_;
		TransformParticle* transform_ = nullptr;
		Emitter* emitter_ = nullptr;
		bool isActive_ = true;
		bool DrawFlag_ = true;
		bool deleteFlag_ = false;
		FrameCountTime timer_;

	public:
		Particle();
		void Initialize();
		static void StaticInitialize();
		void SetComponent(std::unique_ptr<Component> component);
		void SetComponentFront(std::unique_ptr<Component> component);
		void Update();
		static void DrawBefore();
		void Draw();
		~Particle();
	};
}
#pragma once
#include "ConstStruct.h"
#include "Texture.h"
#include <wrl.h>
#include <string>
#include <vector>
#include <d3d12.h>
#include "ConstStruct.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Component.h"

namespace IFE
{
	class Mesh : public Component
	{
		using Component::Component;
	private:
		VertexBuffer<VertexBone>vb_;
		VertexBuffer<Vertex>vt_;
		IndexBuffer ib_;

	public:
		void Initialize()override;
		void Draw()override;
		void Update()override;
		VertexBuffer<VertexBone>* GetVertexBone();
		VertexBuffer<Vertex>* GetVertex();
		IndexBuffer* GetIB();
#ifdef _DEBUG
		void DebugGUI()override {};
		void OutputScene(const std::string& object3d) { object3d; }
#endif
		void LoadingScene(const std::string& object3d) { object3d; }
	};
};

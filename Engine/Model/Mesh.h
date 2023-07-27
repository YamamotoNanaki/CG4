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
		Texture* tex_ = nullptr;

	public:
		void Initialize()override;
		void Draw()override;
		void Update()override;
		VertexBuffer<VertexBone>* GetVertexBone();
		VertexBuffer<Vertex>* GetVertex();
		IndexBuffer* GetIB();
		std::vector<uint32_t> GetIndexArray();
		std::vector<Vertex> GetVertexArray();

		void SetTexture(Texture* tex);
	};
};

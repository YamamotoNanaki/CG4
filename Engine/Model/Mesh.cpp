#include "Mesh.h"
#include "GraphicsAPI.h"
#include "TextureManager.h"
#include <cassert>

using namespace std;
using namespace IFE;

void IFE::Mesh::Initialize()
{
	vb.Initialize();
	vt.Initialize();
	ib.Initialize();
}

void IFE::Mesh::Draw()
{
	static ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//頂点バッファの設定
	if (vb.GetSize() > 0)
	{
		commandList->IASetVertexBuffers(0, 1, vb.GetVBView());
	}
	if (vt.GetSize() > 0)
	{
		commandList->IASetVertexBuffers(0, 1, vt.GetVBView());
	}
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(ib.GetIBView());
	//描画コマンド
	commandList->DrawIndexedInstanced((UINT)ib.GetSize(), 1, 0, 0, 0);
}

void IFE::Mesh::Update()
{
}

VertexBuffer<VertexBone>* IFE::Mesh::GetVertexBone()
{
	return &vb;
}

VertexBuffer<Vertex>* IFE::Mesh::GetVertex()
{
	return &vt;
}

IndexBuffer* IFE::Mesh::GetIB()
{
	return &ib;
}

#include "IndexBuffer.h"
#include "GraphicsAPI.h"

using namespace IFE;

void IFE::IndexBuffer::SetIndex(std::vector<uint32_t>& _indices)
{
	indices = std::move(_indices);
}

void IFE::IndexBuffer::SetIndex(uint32_t* _indices, size_t indexCount)
{
	for (int32_t i = 0; i < indexCount; i++)
	{
		this->indices.push_back(_indices[i]);
	}
}

void IFE::IndexBuffer::Initialize()
{
	// インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint32_t) * indices.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};   // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// インデックスバッファの設定
	D3D12_RESOURCE_DESC resDesc{};  // リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//インデックスバッファの生成
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT result = GraphicsAPI::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp,				//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,				//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	uint32_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	//全インデックスに対して
	for (int32_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];	//インデックスをコピー
	}

	//つながりを解除
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeIB;
}

D3D12_INDEX_BUFFER_VIEW* IFE::IndexBuffer::GetIBView()
{
	return &ibView;
}

size_t IFE::IndexBuffer::GetSize()
{
	return indices.size();
}

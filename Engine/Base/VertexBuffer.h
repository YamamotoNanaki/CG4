#pragma once
#include <vector>
#include <array>
#include <wrl.h>
#include <d3d12.h>
#include <cassert>
#include "GraphicsAPI.h"

namespace IFE
{
	template <class T>
	class VertexBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
		std::vector<T> vertices;
		D3D12_VERTEX_BUFFER_VIEW vbView{};

	public:
		void Reset();
		void SetVerticle(std::vector<T>& _vertices);
		void SetVerticle(T* _vertices, size_t vertexCount);
		void Initialize();
		void Transfer(T* _vertices, size_t vertexCount);
		void Transfer(std::vector<T>& _vertices);
		D3D12_VERTEX_BUFFER_VIEW* GetVBView();
		size_t GetSize();
	};

	template<class T>
	inline void VertexBuffer<T>::Reset()
	{
		vertices.clear();
	}

	template<class T>
	inline void VertexBuffer<T>::SetVerticle(std::vector<T>& _vertices)
	{
		void Reset();
		vertices = std::move(_vertices);
	}

	template<class T>
	inline void VertexBuffer<T>::SetVerticle(T* _vertices, size_t vertexCount)
	{
		void Reset();
		for (int32_t i = 0; i < vertexCount; i++)
		{
			this->vertices.push_back(_vertices[i]);
		}
	}

	template<class T>
	inline void VertexBuffer<T>::Initialize()
	{
		if (vertices.size() == 0)return;
		// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
		UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

		// 頂点バッファの設定
		D3D12_HEAP_PROPERTIES heapProp{};   // ヒープ設定
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

		D3D12_RESOURCE_DESC resDesc{};  // リソース設定
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // 頂点データ全体のサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 頂点バッファの生成
		ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();
		HRESULT result = device->CreateCommittedResource(
			&heapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE, &resDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
		assert(SUCCEEDED(result));
		// GPU上のバッファに対応した仮想メモリを取得
		T* vertMap = nullptr;
		result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));

		// 全頂点に対して
		for (int32_t i = 0; i < vertices.size(); i++)
		{
			vertMap[i] = vertices[i];   // 座標をコピー
		}

		// マップを解除
		vertBuff->Unmap(0, nullptr);

		// 頂点バッファビューの作成

		vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeVB;
		vbView.StrideInBytes = sizeof(vertices[0]);
	}
	template<class T>
	inline void VertexBuffer<T>::Transfer(T* _vertices, size_t vertexCount)
	{
		T* vertMap = nullptr;
#ifdef _DEBUG
		HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
#else
		vertBuff->Map(0, nullptr, (void**)&vertMap);
#endif

		// 全頂点に対して
		for (int32_t i = 0; i < vertexCount; i++)
		{
			vertMap[i] = _vertices[i];   // 座標をコピー
		}

		// マップを解除
		vertBuff->Unmap(0, nullptr);
		Reset();
		SetVerticle(_vertices, vertexCount);
	}
	template<class T>
	inline void VertexBuffer<T>::Transfer(std::vector<T>& _vertices)
	{
		T* vertMap = nullptr;
#ifdef _DEBUG
		HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
#else
		vertBuff->Map(0, nullptr, (void**)&vertMap);
#endif

		// 全頂点に対して
		for (int32_t i = 0; i < _vertices.size(); i++)
		{
			vertMap[i] = _vertices[i];   // 座標をコピー
		}

		// マップを解除
		vertBuff->Unmap(0, nullptr);
		vertices = std::move(_vertices);
	}
	template<class T>
	inline D3D12_VERTEX_BUFFER_VIEW* VertexBuffer<T>::GetVBView()
	{
		return &vbView;
	}
	template<class T>
	inline size_t VertexBuffer<T>::GetSize()
	{
		return vertices.size();
	}
}

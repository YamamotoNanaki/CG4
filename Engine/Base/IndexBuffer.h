#pragma once
#include <vector>
#include <wrl.h>
#include <d3d12.h>

namespace IFE
{
	class IndexBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
		std::vector<uint32_t> indices;
		D3D12_INDEX_BUFFER_VIEW ibView{};
	public:
		void SetIndex(std::vector<uint32_t>& indices);
		void SetIndex(uint32_t* indices, size_t indexCount);
		void Initialize();
		D3D12_INDEX_BUFFER_VIEW* GetIBView();
		size_t GetSize();
	};
}

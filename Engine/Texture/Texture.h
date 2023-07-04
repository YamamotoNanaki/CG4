#pragma once
#pragma warning(push)
#pragma warning(disable:26812)
#include <DirectXTex.h>
#pragma warning(pop)
#include <d3d12.h>
#include <wrl.h>
#include <string>

namespace IFE
{
	struct Texture
	{
	public:
		Microsoft::WRL::ComPtr<ID3D12Resource> texbuff_ = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle_{};
		D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle_{};
		std::string texName_{};
		bool free_ = false;

	private:
		Texture* ptr_;

	public:
		Texture();
		void SetTexture(uint32_t rootParameterIndex);
		Texture* GetPtr()const;
	};
}

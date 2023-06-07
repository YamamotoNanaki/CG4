#pragma once
#pragma warning(push)
#pragma warning(disable:26812)
#include <DirectXTex.h>
#pragma warning(pop)
#include <d3d12.h>
#include <wrl.h>
#include <string>

#pragma comment(lib,"d3d12.lib")

namespace IFE
{
	struct Texture
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> texbuff = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle{};
		D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle{};
		std::string texName{};
		bool free = false;

	private:
		Texture* ptr_;

	public:
		Texture();
		void SetTexture(UINT rootParameterIndex);
		Texture* GetPtr()const;
	};
}

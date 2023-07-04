#include "Texture.h"
#include "TextureManager.h"
#include "GraphicsAPI.h"

#pragma comment(lib,"d3d12.lib")

using namespace IFE;

IFE::Texture::Texture()
{
	ptr_ = this;
}

void IFE::Texture::SetTexture(uint32_t rootParameterIndex)
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	ID3D12DescriptorHeap* heaps[] = { TextureManager::Instance()->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	commandList->SetGraphicsRootDescriptorTable((UINT)rootParameterIndex, GPUHandle_);
}

Texture* IFE::Texture::GetPtr()const
{
	return ptr_;
}

#include "Texture.h"
#include "TextureManager.h"
#include "GraphicsAPI.h"

using namespace IFE;

IFE::Texture::Texture()
{
	ptr_ = this;
}

void IFE::Texture::SetTexture(UINT rootParameterIndex)
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	ID3D12DescriptorHeap* heaps[] = { TextureManager::Instance()->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	commandList->SetGraphicsRootDescriptorTable(rootParameterIndex, GPUHandle);
}

Texture* IFE::Texture::GetPtr()const
{
	return ptr_;
}

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
	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint32_t) * indices.size());

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};   // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	D3D12_RESOURCE_DESC resDesc{};  // ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//�C���f�b�N�X�o�b�t�@�̐���
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT result = GraphicsAPI::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp,				//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,				//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	uint32_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	//�S�C���f�b�N�X�ɑ΂���
	for (int32_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];	//�C���f�b�N�X���R�s�[
	}

	//�Ȃ��������
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

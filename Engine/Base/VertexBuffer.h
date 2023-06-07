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
		// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
		UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

		// ���_�o�b�t�@�̐ݒ�
		D3D12_HEAP_PROPERTIES heapProp{};   // �q�[�v�ݒ�
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

		D3D12_RESOURCE_DESC resDesc{};  // ���\�[�X�ݒ�
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// ���_�o�b�t�@�̐���
		ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();
		HRESULT result = device->CreateCommittedResource(
			&heapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE, &resDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
		assert(SUCCEEDED(result));
		// GPU��̃o�b�t�@�ɑΉ��������z���������擾
		T* vertMap = nullptr;
		result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));

		// �S���_�ɑ΂���
		for (int32_t i = 0; i < vertices.size(); i++)
		{
			vertMap[i] = vertices[i];   // ���W���R�s�[
		}

		// �}�b�v������
		vertBuff->Unmap(0, nullptr);

		// ���_�o�b�t�@�r���[�̍쐬

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

		// �S���_�ɑ΂���
		for (int32_t i = 0; i < vertexCount; i++)
		{
			vertMap[i] = _vertices[i];   // ���W���R�s�[
		}

		// �}�b�v������
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

		// �S���_�ɑ΂���
		for (int32_t i = 0; i < _vertices.size(); i++)
		{
			vertMap[i] = _vertices[i];   // ���W���R�s�[
		}

		// �}�b�v������
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

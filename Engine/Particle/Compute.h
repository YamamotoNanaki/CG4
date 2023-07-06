#pragma once
#include "GraphicsPipelineManager.h"
#include "GraphicsAPI.h"
#include <d3d12.h>
#include <d3dcompiler.h>
#include <cassert>
#include <wrl.h>

namespace IFE
{
	template <class T>
	class Compute
	{
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator_;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_ = nullptr;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate_ = nullptr;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_ = nullptr;
		size_t size_;

	public:
		T* data = nullptr;
		std::string name_;

	public:
		void Initialize(const std::string& name, size_t size = 1, const std::string& shaderName = "testCS.hlsl");
		void Execution();
	};
	template<class T>
	inline void Compute<T>::Initialize(const std::string& name, size_t size, const std::string& shaderName)
	{
		name_ = name;
		size_ = size;
		auto gpm = GraphicsPipelineManager::Instance();
		ID3D12Device* device = GraphicsAPI::GetDevice();
		std::string cs = "Data/Shaders/" + shaderName;
		gpm->ShaderCompile(cs, SHADER_COMPILE_SETTINGS::Compute);

		ID3DBlob* shader = gpm->GetBlob(3);
		ID3DBlob* signature = nullptr;

		auto result = D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(),
			D3D_BLOB_ROOT_SIGNATURE, 0, &signature);
		assert(SUCCEEDED(result));

		//ルートシグネチャの生成
		result = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
			IID_PPV_ARGS(rootsignature_.GetAddressOf()));
		assert(SUCCEEDED(result));

		D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};
		pipelineDesc.CS.pShaderBytecode = shader->GetBufferPointer();
		pipelineDesc.CS.BytecodeLength = shader->GetBufferSize();
		pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		pipelineDesc.NodeMask = 0;
		pipelineDesc.pRootSignature = rootsignature_.Get();

		result = device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(pipelinestate_.GetAddressOf()));
		assert(SUCCEEDED(result));

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));
		assert(SUCCEEDED(result));

		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		heapProp.CreationNodeMask = 1;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
		heapProp.VisibleNodeMask = 1;

		//サイズは定数バッファと同じように指定
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Alignment = 0;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.Height = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.MipLevels = 1;
		resourceDesc.SampleDesc = { 1, 0 };
		resourceDesc.Width = (sizeof(T) * size_ + 0xff) & ~0xff;
		Microsoft::WRL::ComPtr<ID3D12Resource> rsc;
		result = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
			IID_PPV_ARGS(&rsc));
		assert(SUCCEEDED(result));

		D3D12_UNORDERED_ACCESS_VIEW_DESC viewDesc{};
		viewDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		viewDesc.Format = DXGI_FORMAT_UNKNOWN;
		viewDesc.Buffer.NumElements = (UINT)size_;
		viewDesc.Buffer.StructureByteStride = sizeof(T);

		device->CreateUnorderedAccessView(rsc.Get(), nullptr, &viewDesc, heap_->GetCPUDescriptorHandleForHeapStart());

		D3D12_RANGE range{ 0, 1 };
		void* d = data;
		result = rsc->Map(0, &range, &d);
		assert(SUCCEEDED(result));

		result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE,
			IID_PPV_ARGS(allocator_.GetAddressOf()));
		assert(SUCCEEDED(result));
		result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, allocator_.Get(), nullptr,
			IID_PPV_ARGS(cmdList_.GetAddressOf()));
		assert(SUCCEEDED(result));
	}
	template<class T>
	inline void Compute<T>::Execution()
	{
		//コマンドのリセット
		cmdList_->Close();
		auto result = allocator_->Reset(); // キューをクリア
		assert(SUCCEEDED(result));
		result = cmdList_->Reset(allocator_.Get(), nullptr);  // 再びコマンドリストを貯める準備
		assert(SUCCEEDED(result));
		//それぞれのセット
		cmdList_->SetComputeRootSignature(rootsignature_.Get());
		cmdList_->SetPipelineState(pipelinestate_.Get());
		cmdList_->SetDescriptorHeaps(1, heap_.GetAddressOf());

		auto handle = heap_->GetGPUDescriptorHandleForHeapStart();
		cmdList_->SetComputeRootDescriptorTable(0, handle);

		//コンピュートシェーダーの実行(今回は256個のスレッドグループを指定)
		cmdList_->Dispatch((UINT)size_, 1, 1);

		cmdList_->Close();

		//コマンドの実行
		Microsoft::WRL::ComPtr<ID3D12CommandList> com[] = {
		   cmdList_.Get(),
		};
		GraphicsAPI::Instance()->GetCommandQueue()->ExecuteCommandLists(1, com->GetAddressOf());

		auto fence = GraphicsAPI::Instance()->GetFence();
		auto fenceVal = GraphicsAPI::Instance()->GetFenceVal();
		if (fence->GetCompletedValue() != fenceVal)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
}

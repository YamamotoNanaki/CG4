#pragma once
#include "GraphicsAPI.h"
#include "GraphicsPipelineManager.h"
#include <d3d12.h>
#include <d3dcompiler.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include <array>
#include <random>

struct IDs {
	float grpId;
	float grpThrdId;
	float dsptThrdId;
	unsigned int grpIdx;
};

struct SimpleBuffer_t
{
	int		i;
	float	f;
};
std::array<SimpleBuffer_t, 64> indata;
std::vector<IDs> uavdata(2 * 2 * 2 * 4 * 4 * 4);

namespace IFE
{
	template <class T>
	class Compute
	{
		ID3D12CommandAllocator* cmdAlloc_ = nullptr;
		ID3D12GraphicsCommandList* cmdList_ = nullptr;
		ID3D12PipelineState* pipeline_ = nullptr;
		ID3D12RootSignature* rootSignature_ = nullptr;
		ID3D12DescriptorHeap* descriptorHeap_ = nullptr;
		ID3D12CommandQueue* cmdQue_ = nullptr;

		ID3D12Resource* uavBuffer = nullptr;
		ID3D12Resource* inBuffer = nullptr;

	private:
		inline static const std::string defaultDirectory_ = "Data/Shaders/";

	public:
		std::vector<T> data_;
		std::string name_;

	public:
		void Initialize(const std::string& name, const std::vector<T>& data, const std::string& shaderName = "testCS.hlsl");
		void Execute();

	private:
		void CreateUAVBuffer(ID3D12Resource*& res);
		void CreateSRVBuffer(ID3D12Resource*& res);
		void CreateCopyBuffer(ID3D12Resource*& res);
		void CreateRootSignature();
		void CreateComputePipeline(const std::string& shaderName);
		void CreateUAVDescriptorHeap();
		void CreateUAV(ID3D12Resource*& res);
		void CreateSRV(ID3D12Resource*& res);
	};

	template<class T>
	inline void Compute<T>::Initialize(const std::string& name, const std::vector<T>& data, const std::string& shaderName)
	{
		HRESULT result = S_OK;
		name_ = name;
		data_ = data;

		CreateRootSignature();
		CreateComputePipeline(defaultDirectory_ + shaderName);
		CreateUAVDescriptorHeap();

		//コマンドアロケータ作成
		result = GraphicsAPI::GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&cmdAlloc_));
		assert(SUCCEEDED(result));
		//コマンドリスト作成
		result = GraphicsAPI::GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, cmdAlloc_, pipeline_, IID_PPV_ARGS(&cmdList_));
		assert(SUCCEEDED(result));

		D3D12_COMMAND_QUEUE_DESC queDesc = {};
		queDesc.NodeMask = 0;
		queDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queDesc.Priority = 0;
		queDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		result = GraphicsAPI::GetDevice()->CreateCommandQueue(&queDesc, IID_PPV_ARGS(&cmdQue_));
		assert(SUCCEEDED(result));



		CreateUAVBuffer(uavBuffer);
		CreateUAV(uavBuffer);

		CreateSRVBuffer(inBuffer);
		CreateSRV(inBuffer);
	}

	template<class T>
	inline void Compute<T>::Execute()
	{
		std::random_device seed;
		std::mt19937 mt(seed());
		std::uniform_real_distribution<float> distf(0.0, 1.0);
		std::uniform_int_distribution<unsigned int> disti(1, 600);
		for (auto& d : indata) {
			d.f = distf(mt);
			d.i = disti(mt);
		}

		//T* cbuff = nullptr;
		SimpleBuffer_t* cbuff = nullptr;
		inBuffer->Map(0, nullptr, (void**)&cbuff);
		copy(indata.begin(), indata.end(), cbuff);
		inBuffer->Unmap(0, nullptr);

		cmdList_->SetComputeRootSignature(rootSignature_);//ルートシグネチャセット
		ID3D12DescriptorHeap* descHeaps[] = { descriptorHeap_ };
		cmdList_->SetDescriptorHeaps(1, descHeaps);//ディスクリプタヒープのセット

		//ルートパラメータのセット
		cmdList_->SetComputeRootDescriptorTable(0,
			descriptorHeap_->GetGPUDescriptorHandleForHeapStart()
		);
		cmdList_->Dispatch(2, 2, 2);//ディスパッチ


		ID3D12Resource* cpyBuffer = nullptr;
		CreateCopyBuffer(cpyBuffer);

		//バリア
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Transition.pResource = uavBuffer;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
		barrier.Transition.Subresource = 0;
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		cmdList_->ResourceBarrier(1, &barrier);

		cmdList_->CopyResource(cpyBuffer, uavBuffer);

		cmdList_->Close();
		auto fence_ = GraphicsAPI::Instance()->GetFence();
		auto fenceValue_ = GraphicsAPI::Instance()->GetFenceVal();

		ID3D12CommandList* cmdLists[] = { cmdList_ };
		cmdQue_->ExecuteCommandLists(1, cmdLists);
		cmdQue_->Signal(fence_, ++fenceValue_);
		//待ち
		while (fence_->GetCompletedValue() < fenceValue_) { ; }

		IDs* mappedRes = nullptr;
		D3D12_RANGE rng = {};
		rng.Begin = 0;
		rng.End = uavdata.size() * sizeof(float);
		cpyBuffer->Map(0, &rng, (void**)(&mappedRes));
		std::copy_n(mappedRes, uavdata.size(), uavdata.data());
		cpyBuffer->Unmap(0, nullptr);

		auto result = cmdAlloc_->Reset(); // キューをクリア
		assert(SUCCEEDED(result));
		result = cmdList_->Reset(cmdAlloc_, nullptr);  // 再びコマンドリストを貯める準備
		assert(SUCCEEDED(result));
	}

	template<class T>
	inline void Compute<T>::CreateUAVBuffer(ID3D12Resource*& res)
	{
		HRESULT result = S_OK;
		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		D3D12_RESOURCE_DESC resDesc = {};

		//計算結果を書き込む先のバッファを作成
		resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		resDesc.Format = DXGI_FORMAT_UNKNOWN;
		resDesc.Width = sizeof(uavdata[0]) * uavdata.size();//計算結果書き込み先のサイズを計算
		resDesc.DepthOrArraySize = 1;
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Height = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		result = GraphicsAPI::GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, IID_PPV_ARGS(&res));

		assert(SUCCEEDED(result));
	}

	template<class T>
	inline void Compute<T>::CreateSRVBuffer(ID3D12Resource*& res)
	{
		HRESULT result = S_OK;
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);// = {};
		D3D12_RESOURCE_DESC resDesc = {};

		//入力バッファ作成
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resDesc.Format = DXGI_FORMAT_UNKNOWN;
		resDesc.Width = sizeof(indata[0]) * indata.size();//入力サイズを計算
		resDesc.DepthOrArraySize = 1;
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Height = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		result = GraphicsAPI::GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&res));

		assert(SUCCEEDED(result));
	}

	template<class T>
	inline void Compute<T>::CreateCopyBuffer(ID3D12Resource*& res)
	{
		HRESULT result = S_OK;
		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_READBACK;
		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resDesc.Format = DXGI_FORMAT_UNKNOWN;
		resDesc.Width = sizeof(uavdata[0]) * uavdata.size();//コピー先のサイズを計算(UAVと同じ)
		resDesc.DepthOrArraySize = 1;
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Height = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		result = GraphicsAPI::GetDevice()->CreateCommittedResource(&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_COPY_DEST, nullptr,
			IID_PPV_ARGS(&res));
		assert(SUCCEEDED(result));
	}

	template<class T>
	inline void Compute<T>::CreateRootSignature()
	{
		HRESULT result = S_OK;
		ID3DBlob* errBlob = nullptr;
		D3D12_DESCRIPTOR_RANGE range[2] = {};
		range[0].NumDescriptors = 1;//1つ
		range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//u
		range[0].BaseShaderRegister = 0;//u0
		range[0].OffsetInDescriptorsFromTableStart = 0;
		range[0].RegisterSpace = 0;

		range[1].NumDescriptors = 1;//1つ
		range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//t
		range[1].BaseShaderRegister = 0;//t0
		range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		range[1].RegisterSpace = 0;

		D3D12_ROOT_PARAMETER rp[1] = {};
		rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rp[0].DescriptorTable.NumDescriptorRanges = 2;
		rp[0].DescriptorTable.pDescriptorRanges = range;

		D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
		rootSigDesc.NumParameters = 1;
		rootSigDesc.pParameters = rp;
		rootSigDesc.NumStaticSamplers = 0;
		rootSigDesc.pStaticSamplers = nullptr;
		rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;


		Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
		D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errBlob);
		ID3D12RootSignature* rootSignature = nullptr;
		result = GraphicsAPI::GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(result));
		rootSignature_ = rootSignature;
	}

	template<class T>
	inline void Compute<T>::CreateComputePipeline(const std::string& shaderName)
	{
		GraphicsPipelineManager* gpm = GraphicsPipelineManager::Instance();
		gpm->ShaderCompile(shaderName, SHADER_COMPILE_SETTINGS::Compute);
		Microsoft::WRL::ComPtr<ID3DBlob> csBlob = gpm->GetBlob(3);
		D3D12_COMPUTE_PIPELINE_STATE_DESC pldesc = {};
		pldesc.CS.pShaderBytecode = csBlob->GetBufferPointer();
		pldesc.CS.BytecodeLength = csBlob->GetBufferSize();
		pldesc.NodeMask = 0;
		pldesc.pRootSignature = rootSignature_;
		auto result = GraphicsAPI::GetDevice()->CreateComputePipelineState(&pldesc, IID_PPV_ARGS(&pipeline_));
		assert(SUCCEEDED(result));
	}

	template<class T>
	inline void Compute<T>::CreateUAVDescriptorHeap()
	{
		HRESULT result = S_OK;
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NodeMask = 0;
		descHeapDesc.NumDescriptors = 2;//UAV,SRV
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		result = GraphicsAPI::GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptorHeap_));
		assert(SUCCEEDED(result));
	}

	template<class T>
	inline void Compute<T>::CreateUAV(ID3D12Resource*& res)
	{
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;//バッファとして
		uavDesc.Buffer.NumElements = (UINT)uavdata.size();//要素の総数
		uavDesc.Buffer.StructureByteStride = sizeof(uavdata[0]);//1個当たりの大きさ
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		GraphicsAPI::GetDevice()->CreateUnorderedAccessView(res, nullptr, &uavDesc, descriptorHeap_->GetCPUDescriptorHandleForHeapStart());
	}

	template<class T>
	inline void Compute<T>::CreateSRV(ID3D12Resource*& res)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;//バッファとして
		srvDesc.Buffer.NumElements = (UINT)indata.size();//要素の総数
		srvDesc.Buffer.StructureByteStride = sizeof(indata[0]);//1個当たりの大きさ
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		auto handle = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += GraphicsAPI::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		GraphicsAPI::GetDevice()->CreateShaderResourceView(res, &srvDesc, handle);
	}
}

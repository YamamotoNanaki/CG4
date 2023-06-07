#include "TextureManager.h"
#include "GraphicsAPI.h"
#include "Util.h"
#include "JsonManager.h"
#include "Debug.h"
#include "ImguiManager.h"
#include <d3dx12.h>

using namespace IFE;
using namespace DirectX;
using namespace std;

TextureManager::TextureManager()
{
	descRangeSRV.NumDescriptors = 1;															//テクスチャ一つ
	descRangeSRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;									//種別はテクスチャ
	descRangeSRV.BaseShaderRegister = 0;														//0番スロットから
	descRangeSRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

TextureManager* IFE::TextureManager::Instance()
{
	static TextureManager inst;
	return &inst;
}

void IFE::TextureManager::Initialize()
{
	const size_t kMaxSRVCount = 2056;

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

#ifdef _DEBUG
	HRESULT result = GraphicsAPI::GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
#else
	GraphicsAPI::GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

#endif

	textureSize = 0;
	for (int32_t i = 1; i < textureMax; i++) {
		tex[i].texbuff.Reset();
		tex[i].CPUHandle.ptr = 0;
		tex[i].GPUHandle.ptr = 0;
		tex[i].texName.clear();
		tex[i].free = false;
	}
}

Texture* IFE::TextureManager::GetTexture(const std::string filename)
{
	for (uint16_t i = 1; i < 1000; i++)
	{
		if (tex[i].free == false)continue;
		if (tex[i].texName == filename)return &tex[i];
	}
	return nullptr;
}

Texture* IFE::TextureManager::LoadTexture(const std::string filename, int32_t number)
{
	assert(textureSize < textureMax && "ヒープサイズが足りません");

	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	uint16_t num = 0;
	if (number == -1)
	{
		for (uint16_t i = 1; i < 1000; i++)
		{
			if (tex[i].free == false)continue;
			if (tex[i].texName == filename)return &tex[i];
		}

		for (uint16_t i = 1; i < 1000; i++)
		{
			if (tex[i].free == false)
			{
				num = i;
				break;
			}
		}
	}
	else
	{
		num = (uint16_t)number;
	}

	Texture newtex;
	newtex.texName = filename;
	string file = "Data/Resources/Texture/" + filename;
	wchar_t szFile[256];
	MultiByteToWideChar(CP_ACP, 0, file.c_str(), -1, szFile, _countof(szFile));


	HRESULT result = LoadFromWICFile(
		szFile, WIC_FLAGS_NONE, &metadata, scratchImg);

	assert(SUCCEEDED(result));

	ID3D12Device* device = GraphicsAPI::GetDevice();

	ID3D12GraphicsCommandList* cmdList = GraphicsAPI::Instance()->GetCmdList();
	ID3D12CommandQueue* cmdQueue = GraphicsAPI::Instance()->GetCommandQueue();
	ID3D12Fence* fence = GraphicsAPI::Instance()->GetFence();
	UINT64& fenceVal = GraphicsAPI::Instance()->GetFenceVal();

	const Image* img = scratchImg.GetImage(0, 0, 0);

	D3D12_HEAP_PROPERTIES uploadHeapProp{};
	uploadHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	uploadHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	uploadHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	uploadHeapProp.CreationNodeMask = 0;
	uploadHeapProp.VisibleNodeMask = 0;


	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * img->height;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;

	ID3D12Resource* uploadbuff = nullptr;
	result = device->CreateCommittedResource(
		&uploadHeapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadbuff)
	);

	assert(SUCCEEDED(result));

	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	texHeapProp.CreationNodeMask = 0;
	texHeapProp.VisibleNodeMask = 0;

	resDesc.Format = metadata.format;
	resDesc.Width = metadata.width;
	resDesc.Height = (UINT)metadata.height;
	resDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	resDesc.MipLevels = (UINT16)metadata.mipLevels;
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	result = device->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAG_NONE,
		&resDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&newtex.texbuff));


	uint8_t* mapforImg = nullptr;
	result = uploadbuff->Map(0, nullptr, (void**)&mapforImg);

	assert(SUCCEEDED(result));

	auto srcAddress = img->pixels;
	auto rowPitch = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

	for (int32_t y = 0; y < img->height; ++y)
	{
		std::copy_n(srcAddress, rowPitch, mapforImg);
		srcAddress += img->rowPitch;
		mapforImg += rowPitch;
	}

	uploadbuff->Unmap(0, nullptr);

	D3D12_TEXTURE_COPY_LOCATION src = {};
	src.pResource = uploadbuff;
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Width = (UINT)metadata.width;
	src.PlacedFootprint.Footprint.Height = (UINT)metadata.height;
	src.PlacedFootprint.Footprint.Depth = (UINT)metadata.depth;
	src.PlacedFootprint.Footprint.RowPitch = (UINT)AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
	src.PlacedFootprint.Footprint.Format = img->format;

	D3D12_TEXTURE_COPY_LOCATION dst = {};
	dst.pResource = newtex.texbuff.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;

	cmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = newtex.texbuff.Get();
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;//ここ重要
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;//ここ重要

	cmdList->ResourceBarrier(1, &BarrierDesc);
	result = cmdList->Close();
	assert(SUCCEEDED(result));

	ID3D12CommandList* cmdLists[] = { cmdList };
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	cmdQueue->Signal(fence, ++fenceVal);

	if (fence->GetCompletedValue() != fenceVal)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	ID3D12CommandAllocator* commandAllocator = GraphicsAPI::Instance()->GetCommandAllocator();
	result = commandAllocator->Reset(); // キューをクリア
	assert(SUCCEEDED(result));
	result = cmdList->Reset(commandAllocator, nullptr);  // 再びコマンドリストを貯める準備
	assert(SUCCEEDED(result));
	//}
	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	newtex.CPUHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(srvHeap->GetCPUDescriptorHandleForHeapStart(), num, descriptorSize);
	newtex.GPUHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(srvHeap->GetGPUDescriptorHandleForHeapStart(), num, descriptorSize);
	newtex.free = true;

	tex[num] = newtex;

	D3D12_RESOURCE_DESC resDesc2 = tex[num].texbuff->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//RGBA
	srvDesc.Format = resDesc2.Format;					//画像読み込み
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;		//2dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc2.MipLevels;

	//ヒープの２番目にシェーダーリソースビュー作成
	device->CreateShaderResourceView(
		tex[num].texbuff.Get(),		//ビューと関連付けるバッファ
		&srvDesc,		//テクスチャ設定情報
		tex[num].CPUHandle);

	textureSize++;
	uploadbuff->Release();
	return &tex[num];
}

D3D12_DESCRIPTOR_RANGE& IFE::TextureManager::GetDescRangeSRV()
{
	return descRangeSRV;
}

ID3D12DescriptorHeap* IFE::TextureManager::GetDescriptorHeap()
{
	return srvHeap.Get();
}

#ifdef _DEBUG
void IFE::TextureManager::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();

	static bool load = false;
	static bool search = false;
	im->TextureGUI(&load, &search);
	if (load)
	{
		std::function<void(std::string)> guiFunc = [&](std::string filename)
		{
			LoadTexture(filename);
		};
		im->TextureLoadGUI(guiFunc);
	}
	if (search)
	{
		im->SearchTextureGUI(tex);
	}
	im->ShowTextureGUI(tex);
	im->EndGUI();
}

void IFE::TextureManager::OutputScene()
{
	JsonManager* j = JsonManager::Instance();
	nlohmann::json& js = j->GetJsonData();
	int32_t num = 0;
	for (uint16_t i = 1; i < 1000; i++)
	{
		if (tex[i].free == false)continue;
		js["Texture"][num] = tex[i].texName;
		num++;
	}
	j->OutputAndMakeDirectry("Texture","Texture");
}
#endif

void IFE::TextureManager::LoadingScene()
{
	JsonManager* j = JsonManager::Instance();
	j->Input("Texture/Texture");
	nlohmann::json js = j->GetJsonData();
	for (auto& i : js["Texture"])
	{
		LoadTexture(i);
	}
	j->JsonReset();
}
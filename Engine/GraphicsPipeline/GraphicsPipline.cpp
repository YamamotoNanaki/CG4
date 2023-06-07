#include "GraphicsPipeline.h"
#include <d3dcompiler.h>
#include "Debug.h"
#include "StringUtil.h"
#include "GraphicsAPI.h"
#include "TextureManager.h"
#include <cassert>

using namespace std;
using namespace IFE;

Microsoft::WRL::ComPtr<ID3DBlob> GraphicsPipeline::ErrorBlob = nullptr;

bool IFE::GraphicsPipeline::CreateGraphicsPpipeline(std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout,
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc)
{
	ID3D12Device* device = GraphicsAPI::GetDevice();
	ID3DBlob* rootSigBlob = nullptr;
	HRESULT result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &ErrorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		string error;
		error.resize(ErrorBlob->GetBufferSize());

		copy_n((char*)ErrorBlob->GetBufferPointer(), ErrorBlob->GetBufferSize(), error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(SUCCEEDED(result));
		return true;
	}
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	pipelineDesc.pRootSignature = rootsignature.Get();

	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelinestate));
	return false;
}

bool IFE::GraphicsPipeline::ShaderCompile(std::string shaderName, SHADER_COMPILE_SETTINGS setting)
{
	const static LPCSTR setting_shader_type[3] = { "vs_5_0","ps_5_0","gs_5_0" };
	size_t num = static_cast<size_t>(setting);
	std::wstring wname = StringToWString(shaderName);
	HRESULT result = D3DCompileFromFile(wname.c_str(),  // シェーダファイル名
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", setting_shader_type[num], // エントリーポイント名、シェーダーモデル	指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &blobs[num], &ErrorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		string error;
		error.resize(ErrorBlob->GetBufferSize());

		copy_n((char*)ErrorBlob->GetBufferPointer(), ErrorBlob->GetBufferSize(), error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		return true;
	}
	return false;
}

void IFE::GraphicsPipeline::CreateBasicGraphicsPipeLine()
{
	string vs = defaultDirectory + "ModelVS.hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory + "ModelPS.hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);
	string gs = defaultDirectory + "ModelGS.hlsl";
	ShaderCompile(gs, SHADER_COMPILE_SETTINGS::Geometry);
	name = "3dNormal";

	vector<D3D12_ROOT_PARAMETER> rootParams;

	for (size_t i = 0; i < 4; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		//定数用
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//種類
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//デスクリプタレンジ
		rootParamSeed.Descriptor.RegisterSpace = 0;									//デスクリプタレンジ数
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダーから見える
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//種類
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//デスクリプタレンジ
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//デスクリプタレンジ数
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed2);

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.VS.pShaderBytecode = blobs[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blobs[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferSize();
	if (blobs[(size_t)SHADER_COMPILE_SETTINGS::Geometry] != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = blobs[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = blobs[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferSize();
	}
	pipelineDesc.PS.pShaderBytecode = blobs[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blobs[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferSize();
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;		//深度テストを行う
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // 背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可

	D3D12_INPUT_ELEMENT_DESC* layout = new D3D12_INPUT_ELEMENT_DESC[inputLayout.size()];
	for (size_t i = 0; i < inputLayout.size(); i++)
	{
		layout[i] = inputLayout[i];
	}

	pipelineDesc.InputLayout.pInputElementDescs = layout;
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0〜255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//ブレンドを有効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	CreateGraphicsPpipeline(inputLayout, rootSignatureDesc, pipelineDesc);
	delete[] layout;
}

void IFE::GraphicsPipeline::CreateBasic2DGraphicsPipeLine()
{
	string vs = defaultDirectory + "SpriteVS.hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory + "SpritePS.hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);
	name = "2dNormal";

	vector<D3D12_ROOT_PARAMETER> rootParams;

	for (size_t i = 0; i < 2; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		//定数用
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//種類
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//デスクリプタレンジ
		rootParamSeed.Descriptor.RegisterSpace = 0;									//デスクリプタレンジ数
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダーから見える
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//種類
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//デスクリプタレンジ
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//デスクリプタレンジ数
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed2);

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.VS.pShaderBytecode = blobs[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blobs[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferSize();
	if (blobs[(size_t)SHADER_COMPILE_SETTINGS::Geometry] != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = blobs[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = blobs[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferSize();
	}
	pipelineDesc.PS.pShaderBytecode = blobs[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blobs[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferSize();
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = false;		//深度テストを行わない
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面をカリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可

	D3D12_INPUT_ELEMENT_DESC* layout = new D3D12_INPUT_ELEMENT_DESC[inputLayout.size()];
	for (size_t i = 0; i < inputLayout.size(); i++)
	{
		layout[i] = inputLayout[i];
	}

	pipelineDesc.InputLayout.pInputElementDescs = layout;
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0〜255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//ブレンドを有効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	CreateGraphicsPpipeline(inputLayout, rootSignatureDesc, pipelineDesc);
	delete[] layout;
}

void IFE::GraphicsPipeline::CreateBasicParticleGraphicsPipeLine()
{
	string vs = defaultDirectory + "ParticleVS.hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory + "ParticlePS.hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);
	string gs = defaultDirectory + "ParticleGS.hlsl";
	ShaderCompile(gs, SHADER_COMPILE_SETTINGS::Geometry);
	name = "ParticleNormal";

	vector<D3D12_ROOT_PARAMETER> rootParams;

	for (size_t i = 0; i < 2; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		//定数用
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//種類
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//デスクリプタレンジ
		rootParamSeed.Descriptor.RegisterSpace = 0;									//デスクリプタレンジ数
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダーから見える
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//種類
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//デスクリプタレンジ
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//デスクリプタレンジ数
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed2);

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.VS.pShaderBytecode = blobs[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blobs[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferSize();
	if (blobs[(size_t)SHADER_COMPILE_SETTINGS::Geometry] != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = blobs[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = blobs[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferSize();
	}
	pipelineDesc.PS.pShaderBytecode = blobs[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blobs[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferSize();
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;		//深度テストを行わない
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面をカリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可

	D3D12_INPUT_ELEMENT_DESC* layout = new D3D12_INPUT_ELEMENT_DESC[inputLayout.size()];
	for (size_t i = 0; i < inputLayout.size(); i++)
	{
		layout[i] = inputLayout[i];
	}

	pipelineDesc.InputLayout.pInputElementDescs = layout;
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0〜255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//ブレンドを有効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	CreateGraphicsPpipeline(inputLayout, rootSignatureDesc, pipelineDesc);
	delete[] layout;
}

void IFE::GraphicsPipeline::SetDrawBlendMode()
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	commandList->SetGraphicsRootSignature(rootsignature.Get());
	commandList->SetPipelineState(pipelinestate.Get());
}

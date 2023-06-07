#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include <array>
#include <vector>
#pragma comment(lib,"d3dcompiler.lib")

namespace IFE
{
	enum class SHADER_COMPILE_SETTINGS
	{
		Vertex, Pixel, Geometry,
	};
	class GraphicsPipeline
	{
	private:
		std::array<Microsoft::WRL::ComPtr<ID3DBlob>, 3> blobs = { nullptr,nullptr,nullptr };
		static Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
		std::string defaultDirectory = "Data/Shaders/";
	private:
		bool CreateGraphicsPpipeline(std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout,
			D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc);

	public:
		std::string name;
		bool ShaderCompile(std::string shaderName, SHADER_COMPILE_SETTINGS setting);
		void CreateBasicGraphicsPipeLine();
		void CreateBasic2DGraphicsPipeLine();
		void CreateBasicParticleGraphicsPipeLine();
		void SetDrawBlendMode();
	};
}

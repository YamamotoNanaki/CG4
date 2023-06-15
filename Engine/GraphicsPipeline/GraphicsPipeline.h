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
		std::array<Microsoft::WRL::ComPtr<ID3DBlob>, 3> blobs_ = { nullptr,nullptr,nullptr };
		static Microsoft::WRL::ComPtr<ID3DBlob> sErrorBlob_;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate_;
		static std::string sDefaultDirectory_;
	private:
		bool CreateGraphicsPpipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout,
			const D3D12_ROOT_SIGNATURE_DESC& rootSignatureDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc);

	public:
		std::string name_;
		bool ShaderCompile(const std::string& shaderName, const SHADER_COMPILE_SETTINGS& setting);
		void CreateBasicGraphicsPipeLine();
		void CreateBasic2DGraphicsPipeLine();
		void CreateBasicParticleGraphicsPipeLine();
		void SetDrawBlendMode();
	};
}

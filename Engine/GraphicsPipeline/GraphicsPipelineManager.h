#pragma once
#include <list>
#include <memory>
#include "GraphicsPipeline.h"

namespace IFE
{
	enum class SHADER_COMPILE_SETTINGS
	{
		Vertex, Pixel, Geometry, Compute
	};
	class GraphicsPpipelineManager
	{
	private:
		std::list<std::unique_ptr<GraphicsPipeline>> pipelineList_;
		Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob_;
		std::string sDefaultDirectory_;
		std::array<Microsoft::WRL::ComPtr<ID3DBlob>, 3> blobs_ = { nullptr,nullptr,nullptr };

	public:
		GraphicsPpipelineManager(const GraphicsPpipelineManager& obj) = delete;
		GraphicsPpipelineManager& operator=(const GraphicsPpipelineManager& obj) = delete;
		static GraphicsPpipelineManager* Instance();

		bool ShaderCompile(const std::string& shaderName, const SHADER_COMPILE_SETTINGS& setting);
		void CreateBasicGraphicsPipeLine();
		void CreateBasic2DGraphicsPipeLine();
		void CreateBasicParticleGraphicsPipeLine();
		bool CreateGraphicsPpipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout,
			const D3D12_ROOT_SIGNATURE_DESC& rootSignatureDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc);

	private:
		GraphicsPpipelineManager() {}
	};
}

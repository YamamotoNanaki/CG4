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
	class GraphicsPipelineManager
	{
	private:
		std::list<std::unique_ptr<GraphicsPipeline>> pipelineList_;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_;
		std::string defaultDirectory_;
		std::array<Microsoft::WRL::ComPtr<ID3DBlob>, 3> blobs_ = { nullptr,nullptr,nullptr };

	public:
		GraphicsPipelineManager(const GraphicsPipelineManager& obj) = delete;
		GraphicsPipelineManager& operator=(const GraphicsPipelineManager& obj) = delete;
		static GraphicsPipelineManager* Instance();

		bool ShaderCompile(const std::string& shaderName, const SHADER_COMPILE_SETTINGS& setting);
		void CreateBasicGraphicsPipeLine();
		void CreateBasic2DGraphicsPipeLine();
		void CreateBasicParticleGraphicsPipeLine();
		bool CreateGraphicsPipeline(const std::string& pipelineName,
			const D3D12_ROOT_SIGNATURE_DESC& rootSignatureDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc);

	private:
		GraphicsPipelineManager() {}
	};
}

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
		std::string defaultDirectory_="Data/Shaders/";
		std::array<Microsoft::WRL::ComPtr<ID3DBlob>, 3> blobs_ = { nullptr,nullptr,nullptr };
		uint8_t nextNum_ = 0;
		uint8_t NumMax_ = 0;

		uint8_t commonConstBufferNum_ = 5;
		uint8_t commonModelConstBufferNum_ = 2;
		uint8_t animModelConstBufferNum_ = 1;
		uint8_t spriteColorConstBufferNum_ = 1;

	public:
		GraphicsPipelineManager(const GraphicsPipelineManager& obj) = delete;
		GraphicsPipelineManager& operator=(const GraphicsPipelineManager& obj) = delete;
		static GraphicsPipelineManager* Instance();

		bool ShaderCompile(const std::string& shaderName, const SHADER_COMPILE_SETTINGS& setting);
		GraphicsPipeline* CreateBasicGraphicsPipeLine();
		GraphicsPipeline* CreateBasic2DGraphicsPipeLine();
		GraphicsPipeline* CreateBasicParticleGraphicsPipeLine();
		bool CreateGraphicsPipeline(const std::string& pipelineName,
			const D3D12_ROOT_SIGNATURE_DESC& rootSignatureDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc);
		GraphicsPipeline* GetGraphicsPipeline(const std::string& name);
		GraphicsPipeline* GetGraphicsPipeline(uint8_t number);

	private:
		GraphicsPipelineManager() {}
	};
}

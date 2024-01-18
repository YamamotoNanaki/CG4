#pragma once
#include "Texture.h"
#include <array>

namespace IFE
{
	class TextureManager
	{
	private:
		static const int16_t sTEX_MAX_ = 1024;
		uint16_t textureSize_ = 1;

		D3D12_DESCRIPTOR_RANGE descRangeSRV_{};
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;
		size_t startCPUAddress_;
		size_t startGPUAddress_;
		size_t descriptorSize_;
		std::array<Texture, sTEX_MAX_> tex_;

	public:
		static TextureManager* Instance();
		void Initialize();
		Texture* GetTexture(const std::string& filename);
		Texture* LoadTexture(const std::string& filename, int32_t number = -1);
		Texture* CreateRanderTexture(const std::string& texname, bool depth = false, ID3D12Resource* shadowBaffer = nullptr);
		Texture* CreateRanderTexture(const std::string& texname, float width, float height, bool depth = false, ID3D12Resource* shadowBaffer = nullptr);
		void TexReset() {};


		//�Q�b�^�[
	public:
		D3D12_DESCRIPTOR_RANGE& GetDescRangeSRV();
		ID3D12DescriptorHeap* GetDescriptorHeap();

	private:
		TextureManager();
		TextureManager(const TextureManager&) {}
		TextureManager& operator=(const TextureManager&) {}
		~TextureManager() {}

	public:
#ifdef NDEBUG
#else
		void DebugGUI();
		void OutputScene();
#endif
		void LoadingScene();
	};
}

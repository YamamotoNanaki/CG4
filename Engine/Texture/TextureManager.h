#pragma once
#include "Texture.h"
#include <array>

namespace IFE
{
	class TextureManager
	{
	private:
		static const int16_t textureMax = 1024;
		uint16_t textureSize = 1;

		D3D12_DESCRIPTOR_RANGE descRangeSRV{};
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
		std::array<Texture, textureMax> tex;

	public:
		static TextureManager* Instance();
		void Initialize();
		Texture* GetTexture(const std::string filename);
		Texture* LoadTexture(const std::string filename, int32_t number = -1);
		void TexReset() {};


		//ÉQÉbÉ^Å[
	public:
		D3D12_DESCRIPTOR_RANGE& GetDescRangeSRV();
		ID3D12DescriptorHeap* GetDescriptorHeap();

	private:
		TextureManager();
		TextureManager(const TextureManager&) {}
		TextureManager& operator=(const TextureManager&) {}
		~TextureManager() {}

	public:
#ifdef _DEBUG
		void DebugGUI();
		void OutputScene();
#endif
		void LoadingScene();
	};
}

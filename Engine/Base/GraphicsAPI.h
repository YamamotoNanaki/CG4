#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include <chrono>
#include "IFMath.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")


namespace IFE
{
	class GraphicsAPI final
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		template<class T> using vector = std::vector<T>;

	private:
		static ID3D12Device* device;
		ComPtr < IDXGIFactory6> dxgiFactory = nullptr;
		ComPtr < IDXGISwapChain4> swapchain = nullptr;
		ComPtr < ID3D12CommandAllocator> commandAllocator = nullptr;
		ComPtr < ID3D12GraphicsCommandList> commandList = nullptr;
		ComPtr < ID3D12CommandQueue> commandQueue = nullptr;
		ComPtr < ID3D12DescriptorHeap> rtvHeaps = nullptr;
		// 裏表の2つ分について
		vector< ComPtr<ID3D12Resource>> backBuffers = vector<ComPtr<ID3D12Resource>>(2);
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
		// フェンスの生成
		ComPtr<ID3D12Fence> fence = nullptr;
		UINT64 fenceVal = 0;
		vector<D3D12_VIEWPORT> viewport{};

	private:
		ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
		ComPtr < ID3D12Resource> depthBuffer = nullptr;

		D3D12_RESOURCE_BARRIER barrierDesc{};
		D3D12_RECT scissorrect{};

		// 3．画面クリア        R  G  B   A
		float clearColor[4] = { 0.1f ,0.25f ,0.5f ,1.0f };

		//FPS関連
	private:
		void InitializeFixFps();
		void UpdateFixFps();
		std::chrono::steady_clock::time_point reference_;

	public:
		static GraphicsAPI* Instance();
		static void Finalize();
		void Initialize(const HWND& hwnd, const uint32_t& window_width, const uint32_t& window_height);
		void DrawBefore();
		void DrawSetViewport(const int32_t& viewportNum = 0);
		void DrawAfter();
		uint32_t SetNewViewPort(const float& width, const float& height, const float& topX, const float& topY, const float& minDepth = 0.0f, const float& maxDepth = 1.0f);
		void SetScissorrect(const float& left, const float& right, const float& top, const float& bottom);

		//ゲッター
	public:
		static ID3D12Device* GetDevice();
		ID3D12GraphicsCommandList* GetCmdList();
		ID3D12CommandQueue* GetCommandQueue();
		ID3D12Fence* GetFence();
		ID3D12CommandAllocator* GetCommandAllocator();
		UINT64& GetFenceVal();

	public:
		void SetClearColor(const Float4& color);
		void SetClearColor(const float& Red, const float& Green, const float& Bule);

	private:
		GraphicsAPI() {};
		GraphicsAPI(const GraphicsAPI&) {};
		~GraphicsAPI() {};
		const GraphicsAPI& operator=(const GraphicsAPI&) {};
	};
}
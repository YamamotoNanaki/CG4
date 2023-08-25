#pragma once
#include <wrl.h>
#include "Sprite.h"
#include "ConstStruct.h"
#include "ConstBuffer.h"

namespace IFE
{
	class OldPostEffect : public Sprite
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource>texBuff[2];
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapSRV;
		Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineStateAdd;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
		ConstBuffer<ConstBufferPostEffect> buffer_;

	public:
		ConstBufferPostEffect* constMapPostEffect = nullptr;

	private:
		static const float clearColor[4];

	public:
		OldPostEffect();
		~OldPostEffect();
		void Draw(bool add = false);
		void Update();
		void Initialize();
		void DrawBefore();
		void DrawAfter();
		/// <summary>
		/// RGB�V�t�g�����܂�
		/// </summary>
		/// <param name="shift">0~0.005�ȓ������傤�Ǘǂ��Ǝv���܂�</param>
		void SetRGBShift(float shift);
		//void SetGrayscale(bool gray);
		void SetSepia(float sepia);

	private:
		void CreateGraphicsPipelineState();
	};
}

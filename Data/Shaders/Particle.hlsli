#include "Common.hlsli"
cbuffer cbuff0:register(b5)
{
	float4 color;				//�F(RGBA)
};

struct VSOutput
{
	float4 pos : POSITION;	//���_���W
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//���_���W
	float2 uv :TEXCOORD;		//uv�l
};

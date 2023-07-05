#include "Common.hlsli"
cbuffer cbuff0:register(b5)
{
	float4 color;				//色(RGBA)
};

struct VSOutput
{
	float4 pos : POSITION;	//頂点座標
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//頂点座標
	float2 uv :TEXCOORD;		//uv値
};

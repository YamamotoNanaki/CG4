#include "Common.hlsli"
cbuffer cbuff0:register(b5)
{
	float4 color;	//êF(RGBA)
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCORD;
};

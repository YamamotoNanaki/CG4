#include "Shadow.hlsli"
#include "Util.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float> shadowMap : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float shadowDepth = shadowMap.Sample(smp, input.uv).r;

    return float4(shadowDepth, shadowDepth, shadowDepth, 1);
}
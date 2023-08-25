#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    //float s = shift;
    float4 texcolor = tex0.Sample(smp, input.uv);
    //texcolor.r = tex0.Sample(smp, input.uv + float2(-s, s)).r;
    //texcolor.g = tex0.Sample(smp, input.uv + float2(0, -s)).g;
    //texcolor.b = tex0.Sample(smp, input.uv + float2(s, 0)).b;

    return texcolor;
}
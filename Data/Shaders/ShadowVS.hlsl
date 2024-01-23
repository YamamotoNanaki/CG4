#include "Shadow.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);

    VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mul(viewPro, world), pos); //���W�ɍs�����Z
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = uv;
    float4 p;
    p = mul(lightVP, wpos);
    p.xyz = p.xyz / p.w;
    p.x = (1.0f + p.x) / 2.0f;
    p.y = (1.0f - p.y) / 2.0f;
    output.posSM = p;
    return output;
}
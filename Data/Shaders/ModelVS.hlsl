#include "Model.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	float4 wnormal = normalize(mul(worldMatrix, float4(normal, 0)));
	float4 wpos = mul(worldMatrix, pos);

	VSOutput output;//�s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mul(viewPro, worldMatrix), pos);	//���W�ɍs�����Z
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;
	return output;
}
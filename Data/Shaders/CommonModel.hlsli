#include "Common.hlsli"

cbuffer ConstBufferDataTransform : register(b5)
{
    float explosion;
    float polygonSize;
    float gravity;
    float rotation;
    bool lightFlag;
    bool toonFlag;
};

cbuffer Material : register(b6)
{
    float4 color : packoffset(c0); //êF(RGBA)
    float3 ambient : packoffset(c1);
    float3 diffuse : packoffset(c2);
    float3 specular : packoffset(c3);
    float alpha : packoffset(c3.w);
};
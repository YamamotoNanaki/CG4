#pragma once
#include "IFMath.h"
#include <cstdint>

namespace IFE
{
	static const int32_t MAX_BONES = 128;
	struct ConstBufferDataTransform
	{
		Matrix viewPro;
		Matrix world;
		Float3 cameraPos;
		float pad;
	};

	struct ConstBufferModelGeometry
	{
		float explosion;
		float polygonSize;
		float gravity;
		float rotation;
		uint32_t lightFlag = 1;
		uint32_t toonFlag = 0;
		uint32_t bllomFlag = 0;
		float pad;
	};

	struct ConstBufferBillboard
	{
		Matrix mat;
		Matrix matBillboard;
	};
	struct ConstBufferMatrix
	{
		Matrix mat;
	};
	struct ConstBufferColor
	{
		Float4 color;
	};
	struct ConstBufferMaterial
	{
		Float4 color;	//�F(RGBA)
		Float3 ambient;
		float pad1;
		Float3 diffuse;
		float pad2;
		Float3 specular;
		float alpha;
	};
	struct ConstBufferPostEffect
	{
		//0~0.005���炢��
		float sigma;
		float NFocusWidth = 0.05f;
		float FFocusWidth = 0.07f;
		float FocusDepth = 0.085f;
	};
	struct ConstBufferDataSkin
	{
		Matrix bones[MAX_BONES];
	};
	struct Vertex
	{
		Float3 pos;			//XYZ���W
		Float3 normal;		//�@���x�N�g��
		Float2 uv;			//UV���W
	};
	struct Vertex2D
	{
		Float3 pos;
		Float2 uv;
	};
	struct VertexPos
	{
		Float3 pos;			//XYZ���W
	};

	const uint32_t MAX_BONE_INDICES = 4;
	struct VertexBone
	{
		Float3 pos{};			//XYZ���W
		Float3 normal{};		//�@���x�N�g��
		Float2 uv{};			//UV���W
		uint32_t boneIndex[MAX_BONE_INDICES]{};
		float boneWeight[MAX_BONE_INDICES]{};
	};
}
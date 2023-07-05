#pragma once
#include "IFMath.h"
#include <cstdint>

namespace IFE
{
	static const int32_t MAX_BONES = 128;
	static const uint32_t MAX_BONE_INDICES = 4;

	//CB
	//0
	struct ConstBufferCamera
	{
		Matrix viewPro;
		Matrix matBillboard;
		Matrix matBillboardY;
		Float3 cameraPos;
		float pad;
	};
	//1
	struct ConstBufferCamera2D
	{
		Matrix pro;
	};
	//3
	struct ConstBufferTime
	{
		float time;
		float deltaTime;
		float pad;
		float pad1;
	};
	//4
	struct ConstBufferTransform
	{
		Matrix world;
	};
	//model5
	struct ConstBufferModelGeometry
	{
		float explosion;
		float polygonSize;
		float gravity;
		float rotation;
		uint32_t lightFlag = 1;
		uint32_t toonFlag = 0;
		float pad;
		float pad1;
	};
	//model6
	struct ConstBufferMaterial
	{
		Float4 color;	//色(RGBA)
		Float3 ambient;
		float pad1;
		Float3 diffuse;
		float pad2;
		Float3 specular;
		float alpha;
	};
	//anim7
	struct ConstBufferDataSkin
	{
		Matrix bones[MAX_BONES];
	};
	//sprite,particle5
	struct ConstBufferColor
	{
		Float4 color;
	};
	//posteffect5
	struct ConstBufferPostEffect
	{
		//0~0.005くらいで
		float shift;
		uint32_t nega = 0;
		uint32_t grayscale = 0;
		float sepia = 0;
	};


	//VS
	struct Vertex
	{
		Float3 pos;			//XYZ座標
		Float3 normal;		//法線ベクトル
		Float2 uv;			//UV座標
	};
	struct Vertex2D
	{
		Float3 pos;
		Float2 uv;
	};
	struct VertexPos
	{
		Float3 pos;			//XYZ座標
	};

	struct VertexBone
	{
		Float3 pos{};			//XYZ座標
		Float3 normal{};		//法線ベクトル
		Float2 uv{};			//UV座標
		uint32_t boneIndex[MAX_BONE_INDICES]{};
		float boneWeight[MAX_BONE_INDICES]{};
	};
}
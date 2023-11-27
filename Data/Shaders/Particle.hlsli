
Texture2D<float4> tex : register(t0);

struct ConstBufferColor
{
	float4 color;				//色(RGBA)
};

StructuredBuffer<ConstBufferColor> colorSB : register(t1);

struct ConstBufferDataTransform
{
	matrix mat;					//3D変換行列
	matrix matBillboard;		//ビルボード行列
};

StructuredBuffer<ConstBufferDataTransform> transformSB : register(t2);

struct VSOutput
{
	float4 pos : POSITION;	//頂点座標
    uint instanceID : TEXCOORD;//インスタンスid
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//頂点座標
    float2 uv : TEXCOORD; //uv値
    uint instanceID : TEXCOORD1; //インスタンスid
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
    float4 target2 : SV_TARGET2;
};

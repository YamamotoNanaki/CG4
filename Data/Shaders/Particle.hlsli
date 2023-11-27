
Texture2D<float4> tex : register(t0);

struct ConstBufferColor
{
	float4 color;				//�F(RGBA)
};

StructuredBuffer<ConstBufferColor> colorSB : register(t1);

struct ConstBufferDataTransform
{
	matrix mat;					//3D�ϊ��s��
	matrix matBillboard;		//�r���{�[�h�s��
};

StructuredBuffer<ConstBufferDataTransform> transformSB : register(t2);

struct VSOutput
{
	float4 pos : POSITION;	//���_���W
    uint instanceID : TEXCOORD;//�C���X�^���Xid
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//���_���W
    float2 uv : TEXCOORD; //uv�l
    uint instanceID : TEXCOORD1; //�C���X�^���Xid
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
    float4 target2 : SV_TARGET2;
};

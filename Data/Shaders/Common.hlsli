cbuffer Camera : register(b0)
{
    matrix viewPro;
    matrix matBillboard;    //ビルボード行列
    matrix matBillboardY;   //Yビルボード行列
    float3 cameraPos;
};

cbuffer Camera2D : register(b1)
{
    matrix mat;
};

static const int LIGHT_MAX = 100;
static const int DLIGHT_NUM = LIGHT_MAX;

struct DLight
{
    float3 lightv;
    float3 lightcolor;
    uint active;
};

static const int PLIGHT_NUM = LIGHT_MAX;

struct PLight
{
    float3 lightpos;
    float3 lightcolor;
    float3 lightatten;
    uint active;
};

static const int SLIGHT_NUM = LIGHT_MAX;

struct SLight
{
    float3 lightv;
    float3 lightpos;
    float3 lightcolor;
    float3 lightatten;
    float2 lightFactorAngleCos;
    uint active;
};

static const int CSHADOW_NUM = LIGHT_MAX;

struct CShadow
{
    float3 shadowv;
    float3 casterpos;
    float distanceCasterLight;
    float3 shadowatten;
    float2 shadowFactorAngleCos;
    uint active;
};

cbuffer LightBuff : register(b2)
{
    float3 ambientColor;
    DLight dLights[DLIGHT_NUM];
    PLight pLights[PLIGHT_NUM];
    SLight sLights[SLIGHT_NUM];
    CShadow cShadows[CSHADOW_NUM];
}

cbuffer Time : register(b3)
{
    float time;
    float deltaTime;
}

cbuffer Transform : register(b4)
{
    matrix worldMatrix;
};
struct Particle
{
    float3 position;
    float speed;
    float3 velocity;
    float time;
};

StructuredBuffer<Particle> inBuff : register(t0);

RWStructuredBuffer<Particle> outBuff : register(u0);

[numthreads(4, 4, 4)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint num = DTid.x * 8 * 8 + DTid.y * 8 + DTid.z;
    outBuff[num].position = inBuff[num].position + inBuff[num].speed * inBuff[num].velocity;
    outBuff[num].speed = inBuff[num].speed;
    outBuff[num].velocity = inBuff[num].velocity;
    outBuff[num].time = inBuff[num].time;
}
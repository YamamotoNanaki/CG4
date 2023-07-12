struct SimpleBuffer_t
{
    int i;
    float f;
};

StructuredBuffer<SimpleBuffer_t> inBuff0 : register(t0);
StructuredBuffer<SimpleBuffer_t> inBuff1 : register(t1);

struct Buffer_t
{   
    float grpId;
    float grpThrdId;
    float dspThrdId; //‚Ğ‚Æ‚Ü‚¸‚±‚ê‚¾‚¯g—p‚·‚é
    uint groupIndex;
};
RWStructuredBuffer<Buffer_t> outBuff : register(u0);
//id‚ğ‘‚«‚Ş‚¾‚¯‚ÌCS
[numthreads(4, 4, 4)]
void main(uint3 dtid : SV_DispatchThreadID)
{
	//‚½‚¾‚½‚¾ID‚ğ‘ã“ü
    outBuff[dtid.x * 8 * 8 + dtid.y * 8 + dtid.z].dspThrdId = dtid.x * 8 * 8 + dtid.y * 8 + dtid.z;
    outBuff[dtid.x * 8 * 8 + dtid.y * 8 + dtid.z].grpId = dtid.z;

}

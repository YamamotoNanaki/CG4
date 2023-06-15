#include "Ease.h"

using namespace IFE;

float IFE::Lerp(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	return change * time + startPos;
}
float IFE::InQuad(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	return change * time * time + startPos;
}
float IFE::OutQuad(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	return -change * time * (time - 2) + startPos;
}
IFE::Float3 IFE::OutQuadFloat3(const Float3& startPos, const Float3& endPos, const FrameCountTime& timer)
{
	return { OutQuad(startPos.x, endPos.x, (float)timer.GetEndTime(), (float)timer.NowTime()),
			 OutQuad(startPos.y, endPos.y, (float)timer.GetEndTime(), (float)timer.NowTime()),
			 OutQuad(startPos.z, endPos.z, (float)timer.GetEndTime(), (float)timer.NowTime()) };
};

IFE::Float3 IFE::LerpFloat3(const Float3& startPos, const Float3& endPos, const FrameCountTime& timer)
{
	return { Lerp(startPos.x, endPos.x, (float)timer.GetEndTime(), (float)timer.NowTime()),
			 Lerp(startPos.y, endPos.y, (float)timer.GetEndTime(), (float)timer.NowTime()),
			 Lerp(startPos.z, endPos.z, (float)timer.GetEndTime(), (float)timer.NowTime()) };
}
IFE::Float3 IFE::OutQuadFloat3(const Float3& startPos, const Float3& endPos, float maxTime, float time)
{
	return { OutQuad(startPos.x, endPos.x, maxTime, time),
			 OutQuad(startPos.y, endPos.y, maxTime, time),
			 OutQuad(startPos.z, endPos.z, maxTime, time) };
}
IFE::Float3 IFE::LerpFloat3(const Float3& startPos, const Float3& endPos, float maxTime, float time)
{
	return { Lerp(startPos.x, endPos.x, maxTime, time),
			 Lerp(startPos.y, endPos.y, maxTime, time),
			 Lerp(startPos.z, endPos.z, maxTime, time) };
}

float IFE::InOutQuad(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime / 2;
	float change = endPos - startPos;
	if (time < 1) return change / 2 * time * time + startPos;
	return -change / 2 * ((--time) * (time - 2) - 1) + startPos;
}
float IFE::OutInRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time) {
	if (time < relayTime) {
		return IFE::OutQuad(relayPos, startPos, relayTime, time);
	}
	else {
		return IFE::InQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}

float IFE::InOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time) {
	if (time < relayTime) {
		return IFE::InQuad(relayPos, startPos, relayTime, time);
	}
	else {
		return IFE::OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
float IFE::OutOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time) {
	if (time < relayTime) {
		return IFE::OutQuad(relayPos, startPos, relayTime, time);
	}
	else {
		return IFE::OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}

float IFE::Lerp(float startPos, float endPos, const FrameCountTime& timer)
{
	return Lerp(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::InQuad(float startPos, float endPos, const FrameCountTime& timer)
{
	return InQuad(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::OutQuad(float startPos, float endPos, const FrameCountTime& timer)
{
	return OutQuad(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::InOutQuad(float startPos, float endPos, const FrameCountTime& timer)
{
	return InOutQuad(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::EaseInBack(float startPos, float endPos, const FrameCountTime& timer)
{
	return EaseInBack(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::EaseInBack2(float startPos, float endPos, const FrameCountTime& timer)
{
	return EaseInBack2(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::EaseOutBounce(float startPos, float endPos, const FrameCountTime& timer)
{
	return EaseOutBounce(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}

float IFE::EaseInBack(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	float c1 = 1.70158f;
	float c3 = c1 + 1.f;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
float IFE::EaseInBack2(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	float c1 = 1.2f;
	float c3 = c1 + 0.8f;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
float IFE::EaseOutBounce(float startPos, float endPos, float time, float maxTime) {
	time /= maxTime;
	float change = endPos - startPos;
	float n1 = 7.5625f;
	float d1 = 2.75f;

	if (time < 1.f / d1) {
		return n1 * time * time + startPos + change;
	}
	else if (time < 2.f / d1) {
		return n1 * (time -= 1.5f / d1) * time + 0.75f + startPos + change;
	}
	else if (time < 2.5f / d1) {
		return n1 * (time -= 2.25f / d1) * time + 0.9375f + startPos + change;
	}
	else {
		return n1 * (time -= 2.625f / d1) * time + 0.984375f + startPos + change;
	}
}

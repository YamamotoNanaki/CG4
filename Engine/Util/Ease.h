#pragma once
#include "IFMath.h"
#include "IFETime.h"
#include "Float3.h"

namespace IFE
{
	/// <summary>
	/// 直線移動
	/// </summary>
	/// <param name="startPos">初期地点</param>
	/// <param name="endPos">終了地点</param>
	/// <param name="maxTime">最大時間</param>
	/// <param name="time">現在の時間</param>
	/// <returns></returns>
	float Lerp(float startPos, float endPos, float maxTime, float time);

	float InQuad(float startPos, float endPos, float maxTime, float time);

	float OutQuad(float startPos, float endPos, float maxTime, float time);

	IFE::Float3 OutQuadFloat3(IFE::Float3 startPos, IFE::Float3 endPos, IFE::FrameCountTime timer);
	IFE::Float3 LerpFloat3(IFE::Float3 startPos, IFE::Float3 endPos, IFE::FrameCountTime timer);
	IFE::Float3 OutQuadFloat3(IFE::Float3 startPos, IFE::Float3 endPos, float maxTime, float time);
	IFE::Float3 LerpFloat3(IFE::Float3 startPos, IFE::Float3 endPos, float maxTime, float time);

	float InOutQuad(float startPos, float endPos, float maxTime, float time);

	float EaseInBack(float startPos, float endPos, float maxTime, float time);

	float EaseInBack2(float startPos, float endPos, float maxTime, float time);

	float EaseOutBounce(float startPos, float endPos, float maxTime, float time);

	float OutInRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);

	float InOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);

	float OutOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);


	float Lerp(float startPos, float endPos, FrameCountTime timer);
	float InQuad(float startPos, float endPos, FrameCountTime timer);
	float OutQuad(float startPos, float endPos, FrameCountTime timer);
	float InOutQuad(float startPos, float endPos, FrameCountTime timer);
	float EaseInBack(float startPos, float endPos, FrameCountTime timer);
	float EaseInBack2(float startPos, float endPos, FrameCountTime timer);
	float EaseOutBounce(float startPos, float endPos, FrameCountTime timer);
}
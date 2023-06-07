#include "IFETime.h"

using namespace std;
using namespace IFE;
using namespace std::chrono;

std::chrono::steady_clock::time_point IFETime::startTime;
std::chrono::steady_clock::time_point IFETime::nowTime;
std::chrono::steady_clock::time_point IFETime::frontTime;
float IFETime::deltaTime;
float IFETime::time;

void IFE::IFETime::Initialize()
{
	startTime = steady_clock::now();
	frontTime = startTime;
	nowTime = startTime;
}

std::chrono::steady_clock::time_point IFE::IFETime::GetNowTime()
{
	return std::chrono::steady_clock::time_point();
}

std::chrono::steady_clock::time_point IFE::IFETime::GetStartTime()
{
	return std::chrono::steady_clock::time_point();
}

void IFE::IFETime::Update()
{
	frontTime = nowTime;
	nowTime = steady_clock::now();
	milliseconds t = duration_cast<milliseconds>(nowTime - startTime);
	milliseconds dt = duration_cast<milliseconds>(nowTime - frontTime);

	time = (float)t.count() / 1000.f;
	deltaTime = (float)dt.count() / 1000.f;
}

void FrameCountTime::Set(int32_t e)
{
	this->end = e;
	timer = 0;
}

void FrameCountTime::TimeSet(int32_t t, int32_t e)
{
	this->timer = t;
	this->end = e;
}

void FrameCountTime::SetIf(int32_t e, bool flag)
{
	if (flag)Set(e);
}

void FrameCountTime::Update()
{
	timer++;
}

void FrameCountTime::SafeDownUpdate()
{
	if (timer > 0)
	{
		timer--;
	}
}

void FrameCountTime::SafeUpdate()
{
	if (IsEnd())
	{
		return;
	}
	timer++;
}

int32_t FrameCountTime::NowTime() const
{
	return timer;
}

bool FrameCountTime::IsEnd() const
{
	return timer >= end;
}

int32_t FrameCountTime::GetEndTime() const
{
	return end;
}
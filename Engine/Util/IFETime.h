#pragma once
#include <chrono>

namespace IFE
{
	class IFETime
	{
		IFETime() {};
		IFETime(const IFETime&) {};
		~IFETime() {};
		const IFETime& operator=(const IFETime&) {};
	private:
		static std::chrono::steady_clock::time_point startTime;
		static std::chrono::steady_clock::time_point nowTime;
		static std::chrono::steady_clock::time_point frontTime;

	public:
		static float deltaTime;
		static float time;

	public:
		static void Initialize();
		static std::chrono::steady_clock::time_point GetNowTime();
		static std::chrono::steady_clock::time_point GetStartTime();
		static void Update();
	};

	class FrameCountTime
	{
	private:
		int32_t timer = 0;
		int32_t end = 0;

	public:
		void Set(int32_t end);
		void TimeSet(int32_t timer, int32_t end);
		void SetIf(int32_t end, bool flag);
		void Update();
		void SafeDownUpdate();
		void SafeUpdate();//タイマーがendの数値を超えてカウントされないようになっています。
		int32_t NowTime()const;
		bool IsEnd()const;
		int32_t GetEndTime()const;
	};
}

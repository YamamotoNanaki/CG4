#pragma once
#include "Component.h"

namespace IFE
{
	class Lantern : public Component
	{
	private:
		inline static uint32_t lightNum_ = 0;
		inline static uint32_t lightNumOffset_ = 60;
		inline static bool lightFlag_ = true;
		uint32_t useLightNum_ = 0;
		bool useLightFlag_ = false;
	public:
		void Initialize();
		void Update();

#ifdef NDEBUG
#else
		void ComponentDebugGUI();
#endif
	};
}

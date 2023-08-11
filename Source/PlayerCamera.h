#pragma once
#include "Component.h"
#include "Player.h"
#include "Input.h"

namespace IFE
{
	class PlayerCamera :public Component
	{
	private:
		Float3 position_;
		Float3 rotation_ = { 11,0,0 };
		inline static float sMaxDistance_ = 50;
		float distance_ = 20;
		inline static Input* sInput_ = Input::Instance();
		float timer_ = 0;
		inline static float sMaxTime_ = 0;
	public:
		inline static float sMinDistance_ = 30;
		inline static Player* sPlayerPtr_ = nullptr;

	public:
		void Initialize()override;
		void Update()override;
#ifdef NDEBUG
#else
		void ComponentDebugGUI()override;
		virtual void OutputComponent(nlohmann::json& json)override;
		virtual void LoadingComponent(nlohmann::json& json)override;
#endif
	};
}

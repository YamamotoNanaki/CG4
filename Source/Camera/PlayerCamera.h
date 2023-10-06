#pragma once
#include "Component.h"
#include "Player.h"
#include "Input.h"


namespace IFE
{
	class PlayerCamera :public Component
	{
	private:
		inline static float sMaxDistance_ = 50;
		float distance_ = 20;
		inline static Input* sInput_ = Input::Instance();
		float timer_ = 0;
		inline static float sMaxTime_ = 0;
		Vector3 frontVec_;
		float shakeTimer_ = 0;
		float moveTimerX_ = 0;
		float moveTimerY_ = 0;
		float moveMaxTime_ = 0.3f;
		float moveMaxTimeY_ = 0.3f;
		bool cameraMoveX_ = false;
		bool cameraMoveY_ = false;
		float oldPadX_ = 0;
		float oldPadY_ = 0;

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
#endif
		virtual void LoadingComponent(nlohmann::json& json)override;
	};
}

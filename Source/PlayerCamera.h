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
		float distance_ = 20;
		inline static Input* sInput_ = Input::Instance();
	public:
		inline static Player* sPlayerPtr_ = nullptr;

	public:
		void Initialize();
		void Update();
	};
}

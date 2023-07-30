#include "OverUI.h"
#include "Transform.h"
#include "IFETime.h"
#include "Input.h"
#include "Scene.h"
#include "Sound.h"

void IFE::OverUI::Initialize()
{
	if (transform2D_)initPos_ = transform2D_->position_;
	auto num = Sound::Instance()->LoadWave("title");
	Sound::Instance()->SetVolume(num, 25);
}

void IFE::OverUI::Update()
{
	if (!soundFlag_)
	{
		soundFlag_ = true;
		Sound::Instance()->SoundPlay("title");
	}
	Float2 pos = initPos_;

	timer_ += IFETime::sDeltaTime_;
	if (timer_ > maxTime_)timer_ -= maxTime_;

	pos.y = initPos_.y + sinf(float(M_PI) * 2 * (timer_ / maxTime_)) * 20;

	transform2D_->position_ = pos;

	Input* input = Input::Instance();
	if (input->KeyDown(Key_Space) || input->PadDown(PADCODE::ABXY))
	{
		Sound::Instance()->StopSound("title");
		Scene::Instance()->SetNextScene("Title");
	}
}

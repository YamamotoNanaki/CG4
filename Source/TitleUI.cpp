#include "TitleUI.h"
#include "Transform.h"
#include "IFETime.h"
#include "Input.h"
#include "Scene.h"
#include "Sound.h"
#include "Rand.h"
#include "FireworkChrysanthemum.h"

using namespace IFE;

void IFE::TitleUI::Initialize()
{
	if (transform2D_)initPos_ = transform2D_->position2D_;
	auto num = Sound::Instance()->LoadWave("title");
	Sound::Instance()->SetVolume(num, 25);
	FireworkChrysanthemum::InitDefaultVelocity();
}

void IFE::TitleUI::Update()
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

	transform2D_->position2D_ = pos;

	Firework();

	Input* input = Input::Instance();
	if (input->GetKeyDown(Key::Space) || input->PadDown(PADCODE::ABXY))
	{
		Scene::Instance()->SetNextScene("Main");
	}
}

void IFE::TitleUI::Firework()
{
	fireworkTimer_ += IFETime::sDeltaTime_;
	if (fireworkTimer_ >= fireworkMaxTime_)
	{
		float x = IFE::IFERand::GetRandF(-40, 40);
		float y = IFE::IFERand::GetRandF(-25, 25);
		float z = IFE::IFERand::GetRandF(100, 110);
		uint8_t c = uint8_t(IFE::IFERand::GetRand(0, 4));
		Float3 pos = { x,y,z };
		for (size_t i = 0; i < 3; i++)
		{
			auto e = ParticleManager::Instance()->Instantiate("Chrysanthemum", pos);
			e->GetComponent<FireworkChrysanthemum>()->StartFirework(i);
			e->GetComponent<FireworkChrysanthemum>()->SetColor(c);
		}
		Sound::Instance()->SoundPlay("Firework", false);
		fireworkTimer_ = 0;
		fireworkMaxTime_ = IFE::IFERand::GetRandF(0.25f, 1.25f);
	}
}

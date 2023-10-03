#include "FireworkChrysanthemum.h"
#include "Emitter.h"
#include "Particle.h"
#include "Transform.h"
#include "ColorBuffer.h"
#include "IFETime.h"
#include "Ease.h"
#include "Rand.h"
#include "LightManager.h"
#include "Sound.h"

using namespace IFE;

void IFE::FireworkChrysanthemum::Update()
{
	if (emitterPtr_->particles_.size() == 0)
	{
		LightManager::Instance()->SetPointLightActive(pointLightNum_, false);
		emitterPtr_->deleteFlag_ = true;
		return;
	}
	uint32_t i = 0;
	float t = emitterPtr_->particles_.front()->timer_;
	float maxt = emitterPtr_->particleMaxTime_;
	float l = Lerp(1, 0, maxt - 0.5f, min(t, maxt - 0.5f)) * 0.8f;
	float s = OutQuad(sScale_, 0, maxt, t);
	float randA = IFERand::GetRandF(-l, l);
	for (auto& itr : emitterPtr_->particles_)
	{
		Float3 vel = velocitys_[i] * speed_ * IFETime::sDeltaTime_;
		vel.y -= gravity_;
		itr->transform_->position_ += vel;
		colors_[i] = baseColor_;
		colors_[i] += Float4(IFERand::GetRandF(-0.2f, 0.2f), IFERand::GetRandF(-0.2f, 0.2f), IFERand::GetRandF(-0.01f, 0.01f), randA);
		itr->GetComponent<ColorBuffer>()->SetColor(colors_[i]);
		itr->transform_->scale_ = s;
		i++;
	}
	gravity_ += 0.1f * IFETime::sDeltaTime_;
	float r = 0.5f;
	float g = 0.2f;
	float b = 0.1f;

	switch (colorSetting_)
	{
	case (uint8_t)ParticleColorSetting::Blue:
		if (t < 1)
		{
			b = OutQuad(1.f, 0.5f, 1.f, max(t - 0.25f, 0));
			g = Lerp(1, 0.2f, 1.f * 3 / 4, min(t, 1.f * 2 / 3));
			r = OutQuad(1, 0.1f, 1.f / 2, min(t, 1.f / 2));
		}
		else
		{
			r = 0.1f;
			g = 0.2f;
			b = 0.5f;
		}
		break;
	default:
		if (t < 1)
		{
			r = OutQuad(1.f, 0.5f, 1.f, max(t - 0.25f, 0));
			g = Lerp(1, 0.2f, 1.f * 3 / 4, min(t, 1.f * 2 / 3));
			b = OutQuad(1, 0.1f, 1.f / 2, min(t, 1.f / 2));
			break;
		}
	}
	float a = OutQuad(1.f, 0, maxt - 0.25f, min(t, maxt - 0.25f));
	baseColor_ = Float4(r, g, b, a);
	speed_ = OutQuad(sStartSpeed_, 0, maxt, t);
	LightManager::Instance()->SetPointLightPos(pointLightNum_, transformParticle_->position_);
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { baseColor_.x,baseColor_.y,baseColor_.z });
	float atten = InQuart(0, 1, maxt, t);
	float attenX = InQuart(0.001f, 1, maxt, t);
	LightManager::Instance()->SetPointLightAtten(pointLightNum_, { attenX,atten,atten });
}

void IFE::FireworkChrysanthemum::StartFirework()
{
	emitterPtr_->isActive_ = true;
	velocitys_.resize(particleMaxNum_);
	colors_.resize(particleMaxNum_);

	for (uint32_t i = 0; i < particleMaxNum_; i++)
	{
		Vector3 velocity = sDefaultVelocity_[i];
		if ((Float3)velocity == Float3::zero())
		{
			velocity = { IFERand::GetRandF(-1.f, 1.f), IFERand::GetRandF(-1.f, 1.f), IFERand::GetRandF(-1.f, 1.f) };
		}
		else
		{
			velocity += { IFERand::GetRandF(-0.15f, 0.15f), IFERand::GetRandF(-0.15f, 0.15f), IFERand::GetRandF(-0.15f, 0.15f) };
		}
		velocity.Normalize();
		velocitys_[i] = velocity;
		auto p = emitterPtr_->AddParticle();
		p->transform_->scale_ = sScale_;
		p->AddComponent<ColorBuffer>();
	}
	speed_ = sStartSpeed_;
	LightManager::Instance()->SetPointLightActive(LightManager::nextPNum_, true);
	LightManager::Instance()->SetPointLightPos(LightManager::nextPNum_, transformParticle_->position_);
	LightManager::Instance()->SetPointLightAtten(LightManager::nextPNum_, { 0.001f,0,0 });
	pointLightNum_ = LightManager::nextPNum_;
	LightManager::nextPNum_++;
	Sound::Instance()->SoundPlay("Firework", false);
}

void IFE::FireworkChrysanthemum::SetColor(uint8_t colorSetting)
{
	colorSetting_ = colorSetting;
}

void IFE::FireworkChrysanthemum::InitDefaultVelocity()
{
	sDefaultVelocity_.clear();
	static const float radius = 1.0f;
	static const float pi = static_cast<float>(M_PI);
	float DIV = sqrtf((float)particleMaxNum_);
	sDefaultVelocity_.resize(particleMaxNum_);
	sDefaultVelocity_[0] = { 0, radius, 0 };

	for (uint32_t i = 1; i < DIV; i++)
	{
		float y = cosf(pi / DIV * i) * radius;
		float r = sinf(pi / DIV * i) * radius;
		for (uint32_t j = 0; j < DIV; j++)
		{
			Vector3 vel = { sinf(2 * pi / DIV * j) * r,y,cosf(2 * pi / DIV * j) * r };
			vel.Normalize();
			sDefaultVelocity_[(i - 1) * (uint32_t)DIV + j] = vel;
		}
	}
	sDefaultVelocity_[particleMaxNum_ - 1] = { 0,-radius,0 };
}

IFE::FireworkChrysanthemum::~FireworkChrysanthemum()
{
	LightManager::Instance()->SetPointLightActive(pointLightNum_, false);
}


#ifdef NDEBUG
#else
#include "ImguiManager.h"
void IFE::FireworkChrysanthemum::ComponentDebugGUI()
{
	ImguiManager* imgui = ImguiManager::Instance();
	if (imgui->NewTreeNode("parameter"))
	{
		std::string s = "color : " + std::to_string(baseColor_.x) + " , " + std::to_string(baseColor_.y) + " , " + std::to_string(baseColor_.z) + " , " + std::to_string(baseColor_.w);
		imgui->TextGUI(s);
		imgui->EndTreeNode();
	}

	imgui->DragFloatGUI(&sStartSpeed_, "static start speed", 0.1f);
	imgui->DragFloatGUI(&sScale_, "static defalt scale", 0.05f);
}

void IFE::FireworkChrysanthemum::OutputComponent(nlohmann::json& json)
{
	json["sStartSpeed"] = sStartSpeed_;
	json["sScale"] = sScale_;
}
#endif

void IFE::FireworkChrysanthemum::LoadingComponent(nlohmann::json& json)
{
	sScale_ = json["sScale"];
	sStartSpeed_ = json["sStartSpeed"];
}

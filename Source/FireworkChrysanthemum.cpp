#include "FireworkChrysanthemum.h"
#include "Emitter.h"
#include "Particle.h"
#include "Transform.h"
#include "ColorBuffer.h"
#include "IFETime.h"
#include "Ease.h"
#include "Rand.h"
#include "LightManager.h"
#include "Collider.h"

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
	timer_ = emitterPtr_->particles_.front()->timer_;
	maxTime_ = emitterPtr_->particleMaxTime_;
	float l = Lerp(1, 0, maxTime_ - 0.5f, min(timer_, maxTime_ - 0.5f)) * 0.8f;
	float s = OutQuad(sScale_, 0, maxTime_, timer_);
	float randA = IFERand::GetRandF(-l, l);
	float speedDeltaTime = speed_ * IFETime::sDeltaTime_;
	for (auto& itr : emitterPtr_->particles_)
	{
		Float3 vel = velocitys_[i] * speedDeltaTime;
		vel.x += IFERand::GetRandF(-0.05f, 0.05f);
		vel.z += IFERand::GetRandF(-0.05f, 0.05f);
		vel.y -= gravity_;
		itr->transform_->position_ += vel;
		colors_[i] = baseColor_;
		colors_[i] += Float4(IFERand::GetRandF(-0.2f, 0.2f), IFERand::GetRandF(-0.2f, 0.2f), IFERand::GetRandF(-0.01f, 0.01f), randA);
		itr->GetComponent<ColorBuffer>()->SetColor(colors_[i]);
		itr->transform_->scale_ = s;
		i++;
	}
	gravity_ += 0.1f * IFETime::sDeltaTime_;

	(this->*ColorTableFunc[colorSetting_])();
	auto col = emitterPtr_->GetComponent<Collider>();
	if (col)
	{
		colliderScale += speedDeltaTime;
		col->SetOffsetScale(colliderScale);
		if (timer_ >= 0.75f)
		{
			col->componentDeleteFlag_ = true;
		}
	}
	speed_ = OutQuad(useSpeed_, 0, maxTime_, timer_);
	LightUpdate();
}

void IFE::FireworkChrysanthemum::StartFirework(const size_t& num)
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
	if (num == 0)
	{
		useSpeed_ = sStartSpeed_;
	}
	else if (num == 1)
	{
		useSpeed_ = sStartSpeed1_;
	}
	else
	{
		useSpeed_ = sStartSpeed2_;
		emitterPtr_->AddComponentBack<Collider>();
		auto col = emitterPtr_->GetComponent<Collider>();
		col->SetNoPushBackFlag(true);
		col->SetOffsetScale(colliderScale);
	}
	speed_ = useSpeed_;

	pointLightNum_ = LightManager::GetPointLightNumber();
	LightManager::Instance()->SetPointLightActive(pointLightNum_, true);
	LightManager::Instance()->SetPointLightPos(pointLightNum_, transformParticle_->position_);
	LightManager::Instance()->SetPointLightAtten(pointLightNum_, { 1,1,1 });
}

void IFE::FireworkChrysanthemum::SetColor(const uint8_t& colorSetting)
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

void IFE::FireworkChrysanthemum::BlueParticle()
{
	float r = sDefaultColor_.z;
	float g = sDefaultColor_.y;
	float b = sDefaultColor_.x;

	if (timer_ < 1)
	{
		b = OutQuad(1.f, sDefaultColor_.x, 1.f, max(timer_ - 0.25f, 0));
		g = Lerp(1, sDefaultColor_.y, 1.f * 3 / 4, min(timer_, 1.f * 2 / 3));
		r = OutQuad(1, sDefaultColor_.z, 1.f / 2, min(timer_, 1.f / 2));
	}
	float a = OutQuad(1.f, 0, maxTime_ - 0.25f, min(timer_, maxTime_ - 0.25f));
	baseColor_ = Float4(r, g, b, a);
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { baseColor_.x,baseColor_.y,baseColor_.z });
}

void IFE::FireworkChrysanthemum::GreenParticle()
{
	float r = sDefaultColor_.z;
	float g = sDefaultColor_.x;
	float b = sDefaultColor_.y;

	if (timer_ < 1)
	{
		g = OutQuad(1.f, sDefaultColor_.x, 1.f, max(timer_ - 0.25f, 0));
		b = Lerp(1, sDefaultColor_.y, 1.f * 3 / 4, min(timer_, 1.f * 2 / 3));
		r = OutQuad(1, sDefaultColor_.z, 1.f / 2, min(timer_, 1.f / 2));
	}
	float a = OutQuad(1.f, 0, maxTime_ - 0.25f, min(timer_, maxTime_ - 0.25f));
	baseColor_ = Float4(r, g, b, a);
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { baseColor_.x,baseColor_.y,baseColor_.z });
}

void IFE::FireworkChrysanthemum::YellowParticle()
{
	float r = sDefaultColor_.x;
	float b = sDefaultColor_.z;

	if (timer_ < 1)
	{
		r = OutQuad(1.f, sDefaultColor_.x, 1.f, max(timer_ - 0.25f, 0));
		b = OutQuad(1, sDefaultColor_.z, 1.f / 2, min(timer_, 1.f / 2));
	}
	float a = OutQuad(1.f, 0, maxTime_ - 0.25f, min(timer_, maxTime_ - 0.25f));
	baseColor_ = Float4(r, r, b, a);
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { baseColor_.x,baseColor_.y,baseColor_.z });
}

void IFE::FireworkChrysanthemum::RedParticle()
{
	float r = sDefaultColor_.x;
	float g = sDefaultColor_.y;
	float b = sDefaultColor_.z;

	if (timer_ < 1)
	{
		r = OutQuad(1.f, sDefaultColor_.x, 1.f, max(timer_ - 0.25f, 0));
		g = Lerp(1, sDefaultColor_.y, 1.f * 3 / 4, min(timer_, 1.f * 2 / 3));
		b = OutQuad(1, sDefaultColor_.z, 1.f / 2, min(timer_, 1.f / 2));
	}
	float a = OutQuad(1.f, 0, maxTime_ - 0.25f, min(timer_, maxTime_ - 0.25f));
	baseColor_ = Float4(r, g, b, a);
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { baseColor_.x,baseColor_.y,baseColor_.z });
}

void IFE::FireworkChrysanthemum::PurpleParticle()
{
	float r = sDefaultColor_.x;
	float g = sDefaultColor_.z;

	if (timer_ < 1)
	{
		r = OutQuad(1.f, sDefaultColor_.x, 1.f, max(timer_ - 0.25f, 0));
		g = OutQuad(1, sDefaultColor_.z, 1.f / 2, min(timer_, 1.f / 2));
	}
	float a = OutQuad(1.f, 0, maxTime_ - 0.25f, min(timer_, maxTime_ - 0.25f));
	baseColor_ = Float4(r, g, r, a);
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { baseColor_.x,baseColor_.y,baseColor_.z });
}

void IFE::FireworkChrysanthemum::LightUpdate()
{
	LightManager::Instance()->SetPointLightPos(pointLightNum_, transformParticle_->position_);
	float atten = InQuint(0, 1, maxTime_, timer_);
	float attenX = InQuint(0.05f, 1, maxTime_, timer_);
	LightManager::Instance()->SetPointLightAtten(pointLightNum_, { attenX,atten,atten });
}

void (FireworkChrysanthemum::* FireworkChrysanthemum::ColorTableFunc[])() =
{
	&FireworkChrysanthemum::BlueParticle,&FireworkChrysanthemum::GreenParticle,&FireworkChrysanthemum::YellowParticle,&FireworkChrysanthemum::RedParticle,
	&FireworkChrysanthemum::PurpleParticle,
};

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
	imgui->DragFloatGUI(&sStartSpeed1_, "static start speed1", 0.1f);
	imgui->DragFloatGUI(&sStartSpeed2_, "static start speed2", 0.1f);
	imgui->DragFloatGUI(&sScale_, "static defalt scale", 0.05f);
}

void IFE::FireworkChrysanthemum::OutputComponent(nlohmann::json& json)
{
	json["sStartSpeed"] = sStartSpeed_;
	json["sStartSpeed1"] = sStartSpeed1_;
	json["sStartSpeed2"] = sStartSpeed2_;
	json["sScale"] = sScale_;
}
#endif

void IFE::FireworkChrysanthemum::LoadingComponent(nlohmann::json& json)
{
	sScale_ = json["sScale"];
	sStartSpeed_ = json["sStartSpeed"];
	sStartSpeed1_ = json["sStartSpeed1"];
	sStartSpeed2_ = json["sStartSpeed2"];
}

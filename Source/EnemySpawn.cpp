#include "EnemySpawn.h"
#include "ObjectManager.h"
#include "SpriteManager.h"
#include "Collider.h"
#include "Player.h"
#include "ImguiManager.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "TextureManager.h"
#include "Transform.h"

void EnemySpawn::Initialize()
{
	for (auto& itr : enemys_)
	{
		if (itr.name == "")
		{
			continue;
		}
		if (!IFE::ObjectManager::Instance()->GetObjectPtr(itr.name))
		{
			continue;
		}
		itr.ptr = IFE::ObjectManager::Instance()->GetObjectPtr(itr.name)->GetComponent<IFE::Enemy>();
		if (!itr.ptr)
		{
			continue;
		}
		itr.ptr->SetHP(itr.hp);
	}
	clook_ = IFE::SpriteManager::Instance()->GetSpritePtr("clook");
	needle_ = IFE::SpriteManager::Instance()->GetSpritePtr("needle");
	timeSprites_[0] = IFE::SpriteManager::Instance()->GetSpritePtr("time");
	timeSprites_[1] = IFE::SpriteManager::Instance()->GetSpritePtr("time1");
	if (clook_)clook_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,0 });
	if (needle_)needle_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,0 });
	if (timeSprites_[0])timeSprites_[0]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,0 });
	if (timeSprites_[1])timeSprites_[1]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,0 });

	for (uint16_t j = 0; j < 10; j++)
	{
		std::string s = "number" + std::to_string(j);
		numberTextures_[j] = IFE::TextureManager::Instance()->GetTexture(s);
	}
}

void EnemySpawn::Update()
{
	enemys_.remove_if([](EnemyData data) {return data.ptr == nullptr; });
	if (!end_ && once_)
	{
		timer_ += IFE::IFETime::sDeltaTime_;
		if (timer_ < 1)
		{
			float a = IFE::InOutQuad(0, 1, 1, timer_);
			clook_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,a });
			needle_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,a });
			timeSprites_[0]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,a });
			timeSprites_[1]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,a });
		}
		else
		{
			clook_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,1 });
			needle_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,1 });
			timeSprites_[0]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,1 });
			timeSprites_[1]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,1 });
		}
		if (timer_ > endTime_ / 2)
		{
			float gb = IFE::InOutQuad(1, 0.25f, endTime_ - endTime_ / 2, timer_ - endTime_ / 2);
			clook_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,gb,gb,1 });
			needle_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,gb,gb,1 });
			timeSprites_[0]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,gb,gb,1 });
			timeSprites_[1]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,gb,gb,1 });
		}
		size_t i = 0;
		for (auto& itr : enemys_)
		{
			if (!itr.flag && timer_ >= itr.startTime)
			{
				itr.ptr->SetLight();
				itr.ptr->objectPtr_->isActive_ = true;
				itr.flag = true;
			}
			if (!itr.end && timer_ >= itr.endTime)
			{
				itr.ptr->SetAction(IFE::EnemyAction::Death);
				itr.end = true;
			}
			if (timer_ > endTime_)
			{
				itr.ptr->SetAction(IFE::EnemyAction::Death);
				itr.end = true;
			}
			i++;
		}

		uint16_t temNum = 10;
		uint16_t scoreTem = uint16_t(30 - timer_);
		for (size_t j = 0; j < 2; j++)
		{
			uint8_t num = uint8_t(scoreTem / temNum);
			timeSprites_[j]->tex_ = numberTextures_[num];
			scoreTem -= num * temNum;
			temNum /= 10;
		}
		if (timer_ > endTime_)
		{
			timer_ = 0;
			end_ = true;
		}

		needle_->GetComponent<IFE::Transform2D>()->rotation2D_ = timer_ * 12;
	}
	if (end_)
	{
		if (timer_ < 0.5f)
		{
			timer_ += IFE::IFETime::sNoScaleDeltaTime_;
			float a = IFE::InOutQuad(1, 0, 0.5f, timer_);
			clook_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,a });
			needle_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,a });
			timeSprites_[0]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,a });
			timeSprites_[1]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,a });
		}
		else
		{
			clook_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,0 });
			needle_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,0 });
			timeSprites_[0]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,0 });
			timeSprites_[1]->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,0 });
		}
	}
}

void EnemySpawn::OnColliderHit(IFE::Collider* collider)
{
	if (collider->objectPtr_->GetComponent<IFE::Player>())
	{
		if (!once_)
		{
			once_ = true;
		}
	}
}

#ifdef NDEBUG
#else
void EnemySpawn::OutputComponent(nlohmann::json& json)
{
	size_t i = 0;
	for (auto itr : enemys_)
	{
		json[i]["name"] = itr.name;
		json[i]["hp"] = itr.hp;
		json[i]["start"] = itr.startTime;
		json[i]["end"] = itr.endTime;
		i++;
	}
}

void EnemySpawn::ComponentDebugGUI()
{
	auto gui = IFE::ImguiManager::Instance();
	gui->DragFloatGUI(&endTime_, "end time", 0.25f, 1, 1000);
	if (gui->ButtonGUI("Add Data"))
	{
		EnemyData data{};
		enemys_.push_back(data);
	}
	uint32_t i = 0;
	for (auto& itr : enemys_)
	{
		if (gui->NewTreeNode(std::to_string(i) + " : " + (itr.name == "" ? "empty" : itr.name)))
		{
			std::string s;
			s = "name : " + itr.name;
			gui->TextGUI(s);
			s = "hp : " + std::to_string(itr.hp);
			gui->TextGUI(s);
			s = "start : " + std::to_string(itr.startTime);
			gui->TextGUI(s);
			s = "end : " + std::to_string(itr.endTime);
			gui->TextGUI(s);
			if (gui->NewTreeNode("edit data"))
			{
				std::string newName;
				gui->InputTextGUI("name", newName);
				if (gui->ButtonGUI("change"))
				{
					itr.name = newName;
				}
				int32_t hp = itr.hp;
				gui->DragIntGUI(&hp, "hp", 1, 1, 5);
				itr.hp = uint8_t(hp);
				gui->DragFloatGUI(&itr.startTime, "start", 0.25f, 0, endTime_);
				gui->DragFloatGUI(&itr.endTime, "end", 0.25f, 0.25f, endTime_);
				gui->EndTreeNode();
			}
			gui->EndTreeNode();
		}
		i++;
	}
}
#endif

void EnemySpawn::LoadingComponent(nlohmann::json& json)
{
	for (auto itr : json)
	{
		EnemyData data;
		data.name = itr["name"];
		data.hp = itr["hp"];
		data.startTime = itr["start"];
		data.endTime = itr["end"];
		enemys_.push_back(data);
	}
}

#include "EnemySpawn.h"
#include "ObjectManager.h"
#include "Collider.h"
#include "Player.h"
#include "ImguiManager.h"

void EnemySpawn::Initialize()
{
	size_t i = 0;
	for (auto& itr : enemys_)
	{
		if (itr.name == "")
		{
			continue;
		}
		itr.ptr = IFE::ObjectManager::Instance()->GetObjectPtr(itr.name)->GetComponent<IFE::Enemy>();
		if (!itr.ptr)
		{
			continue;
		}
		itr.ptr->SetHP(itr.hp);
		i++;
	}
}

void EnemySpawn::Update()
{
	enemys_.remove_if([](EnemyData data) {return data.ptr == nullptr; });
	if (!end_ && once_)
	{
		timer_ += IFE::IFETime::sDeltaTime_;

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
		if (timer_ > endTime_)end_ = true;
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

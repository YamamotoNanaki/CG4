#include "ComponentManager.h"
#include "ComponentHelp.h"
#include "Object3D.h"

void IFE::ComponentManager::Initialize()
{
	for (auto& itr : componentList_)
	{
		if (itr)itr->INITIALIZE();
	}
}

void IFE::ComponentManager::Update()
{
	componentList_.remove_if([](std::unique_ptr<Component>& com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList_)
	{
		if (itr)itr->Update();
	}
}

void IFE::ComponentManager::Draw()
{
	for (auto& itr : componentList_)
	{
		if (itr)itr->Draw();
	}
}

void IFE::ComponentManager::Finalize()
{
	componentList_.clear();
}

void IFE::ComponentManager::SetObjectPtr(Object3D* p)
{
	objectPtr_ = p;
}

//void IFE::ComponentManager::OnColliderHit(ADXCollider* myCol, ADXCollider* col)
//{
//	for (auto& itr : componentList)
//	{
//		itr->OnColliderHit(myCol, col);
//	}
//}

std::vector<std::string> IFE::ComponentManager::GetAllComponentName()
{
	std::vector<std::string>s;

	for (auto& itr : componentList_)
	{
		s.push_back(itr->GetComponentName());
	}
	return s;
}

#ifdef _DEBUG
void IFE::ComponentManager::DebugGUI()
{
	for (auto& itr : componentList_)
	{
		if (itr)itr->DebugGUI();
	}
}

void IFE::ComponentManager::OutputScene(nlohmann::json& j)
{
	for (auto& itr : componentList_)
	{
		if (itr)itr->OutputScene(j);
	}
}

void IFE::ComponentManager::DebugUpdate()
{
	componentList_.remove_if([](std::unique_ptr<Component>& com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList_)
	{
		std::string name = itr->GetComponentName();
		if (name == "Transform");
		else if (name == "TransferGeometryBuffer");
		else if (name == "Material");
		else if (name.find("_Model") != std::string::npos);
		else if (name=="ColorBuffer");
		else
		{
			continue;
		}
		if (itr)itr->Update();
	}
}
#endif

void IFE::ComponentManager::LoadingScene(nlohmann::json& j, const std::string& comName)
{
	auto com = StringToComponent(comName);
	com->LoadingScene(j[comName]);
	AddComponentBack<Component>(std::unique_ptr<Component>(com));
}

std::string IFE::ComponentManager::SetName(const std::string& name)
{
	std::string n = name;
	if (n.starts_with("class "))
	{
		n.replace(0, 6, "");
	}
	if (n.starts_with("struct "))
	{
		n.replace(0, 7, "");
	}
	if (n.starts_with("IFE::"))
	{
		n.replace(0, 5, "");
	}
	return n;
}

void IFE::ComponentManager::SetSpritePtr(Sprite* p)
{
	spritePtr_ = p;
}
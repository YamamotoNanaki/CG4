#include "JsonManager.h"
#include "ModelManager.h"
#include "PrimitiveModel.h"
#include "FBXModel.h"
#include "Debug.h"
#include "ImguiManager.h"
#include "Component.h"

using namespace IFE;
using namespace std;

ModelManager* IFE::ModelManager::Instance()
{
	static ModelManager inst;
	return &inst;
}

void IFE::ModelManager::Finalize()
{
	Instance()->modelList_.clear();
}

void IFE::ModelManager::Update()
{
	for (auto& itr : modelList_)
	{
		itr->Update();
	}
}

void IFE::ModelManager::Draw()
{
	for (auto& itr : modelList_)
	{
		itr->Draw();
	}
}

void IFE::ModelManager::Add(const std::string& modelName, const AddModelSettings& modelSetting, const std::string& fileName, bool smooth)
{
	std::unique_ptr<Component> buff = nullptr;
	if (modelSetting < AddModelSettings::CreateCube)
	{
		string ft;
		switch (modelSetting)
		{
		case AddModelSettings::LoadOBJ:
			ft = ".obj";
			break;
		case AddModelSettings::LoadGLTF:
			ft = ".gltf";
			break;
		}
		FBXModel* ptr = modelLoader_.FBXLoad(fileName, ft, smooth);
		ptr->Initialize();
		ptr->SetSettings(modelSetting);
		buff = std::unique_ptr<Component>(ptr);
	}
	else
	{
		PrimitiveModel* ptr = new PrimitiveModel;
		switch (modelSetting)
		{
		case AddModelSettings::CreateCube:
			ptr->CreateCube();
			break;
		case AddModelSettings::CreateSquare:
			ptr->CreateSphere();
			break;
		case AddModelSettings::CreateTriangle:
			ptr->CreateTriangle();
			break;
		case AddModelSettings::CreateCircle:
			ptr->CreateCircle();
			break;
		case AddModelSettings::CreateSphere:
			ptr->CreateSphere();
			break;
		}
		buff = std::unique_ptr<Component>(ptr);
	}
	string n = modelName;
	if (n.find("_Model") == std::string::npos)
	{
		n += "_Model";
	}
	buff->SetComponentName(n);
	modelList_.push_back(std::move(buff));
}

Component* IFE::ModelManager::GetModel(const std::string& modelName)
{
	string n = modelName;
	if (n.find("_Model") == std::string::npos)
	{
		n += "_Model";
	}
	for (auto& itr : modelList_)
	{
		if (n == itr->GetComponentName())
		{
			return itr.get();
		}
	}
	return nullptr;
}

void IFE::ModelManager::Reset()
{
	modelList_.clear();
}

#ifdef _DEBUG
void IFE::ModelManager::DebugGUI()
{
	static bool add;
	static bool fdelete;
	ImguiManager* im = ImguiManager::Instance();
	im->ModelManagerGUI(&add, &fdelete);
	if (add)
	{
		static string name;
		static string file;
		static bool smooth;
		static uint16_t settings = 0;
		if (im->ModelAddGUI(&name, &file, &settings, &smooth))
		{
			Add(name, (AddModelSettings)settings, file, smooth);
		}
	}
	for (auto& itr : modelList_)
	{
		itr->DebugGUI();
	}

	im->EndGUI();
}

std::string IFE::ModelManager::GetModelNameGUI()
{
	vector<string> names;
	for (auto& itr : modelList_)
	{
		names.push_back(itr->GetComponentName());
	}
	if (names.size() == 0)
	{
		ImguiManager::Instance()->TextGUI("Model not found");
		return "";
	}
	else if (names.size() == 1)
	{
		ImguiManager::Instance()->TextGUI(names[0]);
		return names[0];
	}
	int32_t num = ImguiManager::Instance()->GetModelNameGUI(names);

	return names[num];
}
void IFE::ModelManager::OutputScene()
{
	JsonManager* jm = JsonManager::Instance();
	nlohmann::json& js = jm->GetJsonData();
	int32_t num = 0;
	for (auto& itr : modelList_)
	{
		js[num] = itr->GetComponentName();
		num++;
	}
	jm->OutputAndMakeDirectry("ModelManager","Model");
	for (auto& itr : modelList_)
	{
		itr->OutputScene("");
	}
}
#endif
void IFE::ModelManager::LoadingScene()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("Model/ModelManager");
	nlohmann::json& js = jm->GetJsonData();
	vector<string>names;
	for (auto& itr : js)
	{
		names.push_back(itr);
	}
	jm->JsonReset();
	for (int32_t i = 0; i < names.size(); i++)
	{
		string s = "Model/" + names[i];
		jm->Input(s);
		string componentName = jm->InputString("componentName");
		string fileName = jm->InputString("fileName");
		AddModelSettings setting = (AddModelSettings)jm->InputUINT("settings");
		Add(names[i], setting, fileName);
		jm->JsonReset();
	}
}
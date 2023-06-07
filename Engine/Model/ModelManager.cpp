#include "JsonManager.h"
#include "ModelManager.h"
#include "PrimitiveModel.h"
#include "FBXModel.h"
#include "Debug.h"
#include "ImguiManager.h"

using namespace IFE;
using namespace std;

ModelManager* IFE::ModelManager::Instance()
{
	static ModelManager inst;
	return &inst;
}

void IFE::ModelManager::Finalize()
{
	ModelManager* inst = Instance();
	for (auto& itr : inst->modelList)
	{
		delete itr;
	}
	inst->modelList.clear();
}

void IFE::ModelManager::Update()
{

	for (auto& itr : modelList)
	{
		itr->Update();
	}
}

void IFE::ModelManager::Draw()
{
	for (auto& itr : modelList)
	{
		itr->Draw();
	}
}

void IFE::ModelManager::Add(std::string modelName, AddModelSettings modelSetting, std::string fileName, bool smooth)
{
	Component* buff = nullptr;
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
		FBXModel* ptr = modelLoader.FBXLoad(fileName, ft, smooth);
		ptr->Initialize();
		ptr->SetSettings(modelSetting);
		buff = ptr;
	}
	else
	{
		PrimitiveModel* b = new PrimitiveModel;
		switch (modelSetting)
		{
		case AddModelSettings::CreateCube:
			b->CreateCube();
			break;
		case AddModelSettings::CreateSquare:
			b->CreateSphere();
			break;
		case AddModelSettings::CreateTriangle:
			b->CreateTriangle();
			break;
		case AddModelSettings::CreateCircle:
			b->CreateCircle();
			break;
		case AddModelSettings::CreateSphere:
			b->CreateSphere();
			break;
		}
		buff = std::move(b);
	}
	string n = modelName;
	if (n.find("_Model") == std::string::npos)
	{
		n += "_Model";
	}
	buff->SetComponentName(n);
	modelList.push_back(std::move(buff));
}

Component* IFE::ModelManager::GetModel(std::string modelName)
{
	string n = modelName;
	if (n.find("_Model") == std::string::npos)
	{
		n += "_Model";
	}
	for (auto& itr : modelList)
	{
		if (n == itr->GetComponentName())
		{
			return itr;
		}
	}
	return nullptr;
}

void IFE::ModelManager::Reset()
{
	for (auto& itr : modelList)
	{
		delete itr;
	}
	modelList.clear();
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
	for (auto& itr : modelList)
	{
		itr->DebugGUI();
	}

	im->EndGUI();
}

std::string IFE::ModelManager::GetModelNameGUI()
{
	vector<string> names;
	for (auto& itr : modelList)
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
	for (auto& itr : modelList)
	{
		js[num] = itr->GetComponentName();
		num++;
	}
	jm->OutputAndMakeDirectry("ModelManager","Model");
	for (auto& itr : modelList)
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
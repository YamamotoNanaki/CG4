#include "SpriteManager.h"
#include "Transform.h"
#include "TextureManager.h"

using namespace std;
using namespace IFE;

SpriteManager* IFE::SpriteManager::Instance()
{
	static SpriteManager inst;
	return &inst;
}

void IFE::SpriteManager::SPRITEInitialize()
{
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		itr->SPRITEInitialize();
	}
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		itr->SPRITEInitialize();
	}
}

void IFE::SpriteManager::Initialize()
{
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		itr->Initialize();
	}
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		itr->Initialize();
	}
}

void IFE::SpriteManager::Update()
{
	backgroundList.remove_if([](unique_ptr<Sprite>& spr) {return spr->deleteFlag; });
	foregroundList.remove_if([](unique_ptr<Sprite>& spr) {return spr->deleteFlag; });
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		itr->Update();
	}
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		itr->Update();
	}
}

void IFE::SpriteManager::ForeDraw()
{
	Sprite::DrawBefore();
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		itr->Draw();
	}
}

void IFE::SpriteManager::BackDraw()
{
	Sprite::DrawBefore();
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		itr->Draw();
	}
}

void IFE::SpriteManager::AddInitialize(std::string spriteName, std::string textureName)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	ptr->SPRITEInitialize();
	ptr->spriteName = spriteName;
	ptr->SetTexture(textureName);
	foregroundList.push_back(std::move(ptr));
}

Sprite* IFE::SpriteManager::Add(std::string spriteName)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	ptr->spriteName = spriteName;
	foregroundList.push_back(std::move(ptr));
	return foregroundList.back().get();
}

Sprite* IFE::SpriteManager::AddBackGround(std::string spriteName)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	ptr->spriteName = spriteName;
	backgroundList.push_back(std::move(ptr));
	return backgroundList.back().get();
}

Sprite* IFE::SpriteManager::GetSpritePtr(std::string spriteName)
{
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		if (itr->spriteName == spriteName)
		{
			return itr.get();
		}
	}
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		if (itr->spriteName == spriteName)
		{
			return itr.get();
		}
	}
	return nullptr;
}

Sprite* IFE::SpriteManager::Instantiate(Sprite* gameObject, Float2 position, float rotation)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	*ptr = *gameObject;
	string objectName = GetNewName(gameObject->spriteName);
	ptr->spriteName = objectName;
	foregroundList.push_back(std::move(ptr));
	Sprite* obj = foregroundList.back().get();
	obj->transform->position = position;
	obj->transform->rotation = rotation;
	return obj;
}

Sprite* IFE::SpriteManager::Instantiate(Sprite* gameObject, Float2 position)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	*ptr = *gameObject;
	string objectName = GetNewName(gameObject->spriteName);
	ptr->spriteName = objectName;
	foregroundList.push_back(std::move(ptr));
	Sprite* obj = foregroundList.back().get();
	obj->transform->position = position;
	return obj;
}

Sprite* IFE::SpriteManager::Instantiate(Sprite* gameObject)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	*ptr = *gameObject;
	string objectName = GetNewName(gameObject->spriteName);
	ptr->spriteName = objectName;
	foregroundList.push_back(std::move(ptr));
	Sprite* obj = foregroundList.back().get();
	return obj;
}

void IFE::SpriteManager::SetTexture(std::string spriteName, std::string texName)
{
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		if (itr->spriteName == spriteName)
		{
			itr->SetTexture(texName);
			return;
		}
	}
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		if (itr->spriteName == spriteName)
		{
			itr->SetTexture(texName);
			return;
		}
	}
}

std::list<std::unique_ptr<Sprite>>* IFE::SpriteManager::GetBackgroundList()
{
	return &backgroundList;
}

std::list<std::unique_ptr<Sprite>>* IFE::SpriteManager::GetForegroundList()
{
	return &foregroundList;
}

void IFE::SpriteManager::Reset()
{
	foregroundList.clear();
	backgroundList.clear();
}

bool IFE::SpriteManager::SearchName(std::string name)
{
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		if (itr->spriteName == name)return true;
	}
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		if (itr->spriteName == name)return true;
	}
	return false;
}

#include "JsonManager.h"
#ifdef _DEBUG
#include "ImguiManager.h"
#include "Imgui.h"
void IFE::SpriteManager::DebugGUI()
{
	static bool add = false;
	static bool fdelete = false;
	static bool back = false;
	static bool fmove = false;
	ImGui::Begin("SpriteManager", (bool*)false, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("add"))
		{
			ImGui::MenuItem("Add", "", &add);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("delete"))
		{
			ImGui::MenuItem("delete", "", &fdelete);
			ImGui::EndMenu();
		}
		if (back)
		{
			if (ImGui::BeginMenu("fore ground list"))
			{
				ImGui::MenuItem("fore ground list", "", &back);
				ImGui::EndMenu();
			}
		}
		else
		{
			if (ImGui::BeginMenu("back ground list"))
			{
				ImGui::MenuItem("back ground list", "", &back);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}
	if (add)
	{
		if (ImGui::CollapsingHeader("New Sprite"))
		{
			static char name[256];
			static char tex[256];
			ImGui::InputText("New Sprite Name", name, sizeof(name));
			ImGui::InputText("Set Texture Name", tex, sizeof(tex));
			if (ImGui::Button("Add Sprite"))
			{
				if (SearchName(name))
				{
					ImGui::Text("error : an object with the same name exists");
				}
				else
				{
					AddInitialize(name, tex);
					ImGui::Text("success");
				}
			}
		}
	}
	std::string str;
	if (back)
	{
		for (unique_ptr<Sprite>& itr : backgroundList)
		{
			std::string s;
			itr->DebugGUI(fdelete, fmove, &s);
			if (s != "")
			{
				str = s;
			}
		}
		ImGui::End();
		for (unique_ptr<Sprite>& itr : backgroundList)
		{
			itr->ComponentGUI();
		}
		for (unique_ptr<Sprite>& itr : backgroundList)
		{
			if (itr->spriteName == str)
			{
				foregroundList.push_back(std::move(itr));
				backgroundList.remove(itr);
				break;
			}
		}
	}
	else
	{
		for (unique_ptr<Sprite>& itr : foregroundList)
		{
			std::string s;
			itr->DebugGUI(fdelete, fmove, &s);
			if (s != "")
			{
				str = s;
			}
		}
		ImGui::End();
		for (unique_ptr<Sprite>& itr : foregroundList)
		{
			itr->ComponentGUI();
		}
		for (unique_ptr<Sprite>& itr : foregroundList)
		{
			if (itr->spriteName == str)
			{
				foregroundList.push_back(std::move(itr));
				backgroundList.remove(itr);
				break;
			}
		}
	}
}

void IFE::SpriteManager::OutputScene()
{
	JsonManager* jm = JsonManager::Instance();
	int32_t i = 0;
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		nlohmann::json& js = jm->GetJsonData();
		js[i] = itr->spriteName;
		i++;
	}
	jm->OutputAndMakeDirectry("SpriteManager", "Sprite");
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		nlohmann::json& js = jm->GetJsonData();
		vector<string>names = itr->GetAllComponentName();
		for (int32_t j = 0; j < names.size(); j++)
		{
			js["com"][j] = names[j];
		}
		js["tex"] = itr->tex->texName;
		jm->OutputAndMakeDirectry(itr->spriteName, "Sprite");
	}
	i = 0;
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		nlohmann::json& js = jm->GetJsonData();
		js[i] = itr->spriteName;
		i++;
	}
	jm->OutputAndMakeDirectry("BackGroundManager", "Sprite");
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		nlohmann::json& js = jm->GetJsonData();
		vector<string>names = itr->GetAllComponentName();
		for (int32_t j = 0; j < names.size(); j++)
		{
			js["com"][j] = names[j];
		}
		string s = "Back" + itr->spriteName;
		jm->OutputAndMakeDirectry(s, "Sprite");
	}
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		itr->OutputScene();
	}
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		itr->OutputScene();
	}
}

void IFE::SpriteManager::DebugUpdate()
{
	foregroundList.remove_if([](unique_ptr<Sprite>& obj) {return obj->deleteFlag; });
	backgroundList.remove_if([](unique_ptr<Sprite>& obj) {return obj->deleteFlag; });
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		itr->DebugUpdate();
	}
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		itr->DebugUpdate();
	}
}
#endif
#include "ComponentHelp.h"

void IFE::SpriteManager::LoadingScene()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("Sprite/SpriteManager");
	nlohmann::json js = jm->GetJsonData();
	for (auto itr : js)
	{
		Add(itr);
	}

	jm->JsonReset();
	for (unique_ptr<Sprite>& itr : foregroundList)
	{
		string s = "Sprite/" + itr->spriteName;
		jm->Input(s);
		nlohmann::json js2 = jm->GetJsonData();
		vector<string>names;
		for (auto itr2 : js2["com"])
		{
			names.push_back(itr2);
		}
		itr->tex = TextureManager::Instance()->GetTexture(js2["tex"]);
		jm->JsonReset();
		for (int32_t i = 0; i < names.size(); i++)
		{
			//‚ ‚Æ‚Å•Ê‚Ìcpp‚É
			Component* base = nullptr;

			base = StringToComponent(names[i]);
			base->LoadingScene(itr->spriteName, names[i]);
			itr->SetComponent(base);
		}
		itr->Initialize();
	}
	jm->Input("Sprite/BackGroundManager");
	js = jm->GetJsonData();
	for (auto itr : js)
	{
		AddBackGround(itr);
	}

	jm->JsonReset();
	for (unique_ptr<Sprite>& itr : backgroundList)
	{
		string s = "Sprite/Back" + itr->spriteName;
		jm->Input(s);
		nlohmann::json js2 = jm->GetJsonData();
		vector<string>names;
		for (auto itr2 : js2["com"])
		{
			names.push_back(itr2);
		}
		itr->tex = TextureManager::Instance()->GetTexture(js2["tex"]);
		jm->JsonReset();
		for (int32_t i = 0; i < names.size(); i++)
		{
			//‚ ‚Æ‚Å•Ê‚Ìcpp‚É
			Component* base = nullptr;

			base = StringToComponent(names[i]);
			base->LoadingScene(itr->spriteName, names[i]);
			itr->SetComponent(base);
		}
		itr->Initialize();
	}
}

std::string IFE::SpriteManager::GetNewName(std::string spriteName)
{
	int32_t i = 0;
	string r;
	while (true)
	{
		string s = spriteName + to_string(i);
		if (SearchName(s))
		{
			i++;
		}
		else
		{
			r = s;
			break;
		}
	}
	return r;
}

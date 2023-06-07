#include "JsonManager.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "Debug.h"
#include "ImguiManager.h"
#include "TransferGeometryBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "Rigidbody.h"
#include "ComponentHelp.h"

using namespace IFE;
using namespace std;

ObjectManager* IFE::ObjectManager::Instance()
{
	static ObjectManager inst;
	return &inst;
}

void IFE::ObjectManager::OBJInitialize()
{
	for (unique_ptr<Object3D>& itr : objectList)
	{
		itr->OBJInitialize();
	}
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		itr->OBJInitialize();
	}
}

void IFE::ObjectManager::Initialize()
{
	for (unique_ptr<Object3D>& itr : objectList)
	{
		itr->Initialize();
	}
}

void IFE::ObjectManager::Update()
{
	objectList.remove_if([](unique_ptr<Object3D>& obj) {return obj->deleteFlag; });
	prefabList.remove_if([](unique_ptr<Object3D>& obj) {return obj->deleteFlag; });
	for (unique_ptr<Object3D>& itr : objectList)
	{
		itr->OBJUpdate();
	}
}

void IFE::ObjectManager::Draw()
{
	Object3D::DrawBefore();
	for (unique_ptr<Object3D>& itr : objectList)
	{
		itr->Draw();
	}
}

void IFE::ObjectManager::AddInitialize(std::string objectName, Component* model)
{
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	ptr->OBJInitialize();
	ptr->SetObjectName(objectName);
	ptr->SetModel(model);
	objectList.push_back(std::move(ptr));
}

Object3D* IFE::ObjectManager::Add(std::string objectName)
{
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	ptr->SetObjectName(objectName);
	objectList.push_back(std::move(ptr));
	return objectList.back().get();
}

Object3D* IFE::ObjectManager::AddPrefab(std::string objectName)
{
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	ptr->SetObjectName(objectName);
	prefabList.push_back(std::move(ptr));
	return prefabList.back().get();
}

Object3D* IFE::ObjectManager::GetObjectPtr(std::string objectName)
{
	for (unique_ptr<Object3D>& itr : objectList)
	{
		if (itr->GetObjectName() == objectName)
		{
			return itr.get();
		}
	}
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		if (itr->GetObjectName() == objectName)
		{
			return itr.get();
		}
	}
	return nullptr;
}

Object3D* IFE::ObjectManager::AddFromPrefab(std::string objectName, std::string prefabName)
{
	Object3D* obj = nullptr;
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		if (itr->GetObjectName() == prefabName)
		{
			std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
			*ptr = *itr;
			ptr->SetObjectName(objectName);
			objectList.push_back(std::move(ptr));
			obj = objectList.back().get();
			for (int32_t i = 0; i < obj->childName.size(); i++)
			{
				std::unique_ptr<Object3D> ptr2 = make_unique<Object3D>();
				for (unique_ptr<Object3D>& itr2 : prefabList)
				{
					if (itr2->GetObjectName() == obj->childName[i])
					{
						string s = objectName + "child" + to_string(i);
						*ptr2 = *itr2;
						ptr2->SetObjectName(s);
						obj->child.push_back(ptr2.get());
						obj->childName.push_back(ptr2->GetObjectName());
						ptr2->parent = obj;
						ptr2->parentName = obj->GetObjectName();
						ptr2->Initialize();
						objectList.push_back(std::move(ptr2));
						break;
					}
				}
			}
			obj->Initialize();
			break;
		}
	}
	return obj;
}

Object3D* IFE::ObjectManager::AddFromPrefab(std::string prefabName)
{
	Object3D* obj = nullptr;
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		if (itr->GetObjectName() == prefabName)
		{
			std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
			*ptr = *itr;
			string objectName = GetNewName(prefabName);
			ptr->SetObjectName(objectName);
			objectList.push_back(std::move(ptr));
			obj = objectList.back().get();
			for (int32_t i = 0; i < obj->childName.size(); i++)
			{
				std::unique_ptr<Object3D> ptr2 = make_unique<Object3D>();
				for (unique_ptr<Object3D>& itr2 : prefabList)
				{
					if (itr2->GetObjectName() == obj->childName[i])
					{
						string s = objectName + "child" + to_string(i);
						*ptr2 = *itr2;
						ptr2->SetObjectName(s);
						obj->child.push_back(ptr2.get());
						obj->childName.push_back(ptr2->GetObjectName());
						ptr2->parent = obj;
						ptr2->parentName = obj->GetObjectName();
						ptr2->Initialize();
						objectList.push_back(std::move(ptr2));
						break;
					}
				}
			}
			obj->Initialize();
			break;
		}
	}
	return obj;
}

Object3D* IFE::ObjectManager::Instantiate(Object3D* gameObject, Float3 position, Quaternion rotate)
{
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	//*ptr = *gameObject;
	ptr->isActive = gameObject->isActive;
	ptr->deleteFlag = gameObject->deleteFlag;
	ptr->DrawFlag = gameObject->DrawFlag;
	std::vector<string>s = gameObject->GetAllComponentName();
	for (int32_t i = 0; i < s.size(); i++)
	{
		if (s[i].find("_Model") != std::string::npos)
		{
			ptr->SetModel(ModelManager::Instance()->GetModel(s[i]));
			continue;
		}
		Component* base;
		base = StringToComponent(s[i]);
		for (auto& itr : gameObject->componentList)
		{
			if (itr->componentName == s[i])
			{
				base->Copy(itr);
				break;
			}
		}
		ptr->AddComponentBack<Component>(base);
	}
	ptr->transform = nullptr;
	string objectName = GetNewName(gameObject->GetObjectName());
	objectList.push_back(std::move(ptr));
	Object3D* obj = objectList.back().get();
	obj->child.clear();
	obj->childName.clear();
	obj->SetObjectName(objectName);
	for (int32_t i = 0; i < gameObject->child.size(); i++)
	{
		std::unique_ptr<Object3D> child = make_unique<Object3D>();
		child->isActive = gameObject->child[i]->isActive;
		child->deleteFlag = gameObject->child[i]->deleteFlag;
		child->DrawFlag = gameObject->child[i]->DrawFlag;
		std::vector<string>cs = gameObject->child[i]->GetAllComponentName();
		for (int32_t j = 0; j < cs.size(); j++)
		{
			if (cs[j].find("_Model") != std::string::npos)
			{
				child->SetModel(ModelManager::Instance()->GetModel(cs[j]));
				continue;
			}
			Component* base;
			base = StringToComponent(cs[j]);
			for (auto& itr : gameObject->child[i]->componentList)
			{
				if (itr->componentName == cs[i])
				{
					base->Copy(itr);
					break;
				}
			}
			child->AddComponentBack<Component>(base);
		}
		child->transform = nullptr;
		objectName = GetNewName(gameObject->child[i]->GetObjectName());
		child->SetObjectName(objectName);
		obj->child.push_back(child.get());
		obj->childName.push_back(child->GetObjectName());
		child->parent = obj;
		child->parentName = obj->GetObjectName();
		child->Initialize();
		objectList.push_back(std::move(child));
	}
	obj->Initialize();
	obj->transform->position = position;
	obj->transform->rotation = rotate;
	return obj;
}

Object3D* IFE::ObjectManager::Instantiate(Object3D* gameObject, Float3 position)
{
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	//*ptr = *gameObject;
	ptr->isActive = gameObject->isActive;
	ptr->deleteFlag = gameObject->deleteFlag;
	ptr->DrawFlag = gameObject->DrawFlag;
	std::vector<string>s = gameObject->GetAllComponentName();
	for (int32_t i = 0; i < s.size(); i++)
	{
		if (s[i].find("_Model") != std::string::npos)
		{
			ptr->SetModel(ModelManager::Instance()->GetModel(s[i]));
			continue;
		}
		Component* base;
		base = StringToComponent(s[i]);
		for (auto& itr : gameObject->componentList)
		{
			if (itr->componentName == s[i])
			{
				base->Copy(itr);
				break;
			}
		}
		ptr->AddComponentBack<Component>(base);
	}
	ptr->transform = nullptr;
	string objectName = GetNewName(gameObject->GetObjectName());
	objectList.push_back(std::move(ptr));
	Object3D* obj = objectList.back().get();
	obj->child.clear();
	obj->childName.clear();
	obj->SetObjectName(objectName);
	for (int32_t i = 0; i < gameObject->child.size(); i++)
	{
		std::unique_ptr<Object3D> child = make_unique<Object3D>();
		child->isActive = gameObject->child[i]->isActive;
		child->deleteFlag = gameObject->child[i]->deleteFlag;
		child->DrawFlag = gameObject->child[i]->DrawFlag;
		std::vector<string>cs = gameObject->child[i]->GetAllComponentName();
		for (int32_t j = 0; j < cs.size(); j++)
		{
			if (cs[j].find("_Model") != std::string::npos)
			{
				child->SetModel(ModelManager::Instance()->GetModel(cs[j]));
				continue;
			}
			Component* base;
			base = StringToComponent(cs[j]);
			for (auto& itr : gameObject->child[i]->componentList)
			{
				if (itr->componentName == cs[i])
				{
					base->Copy(itr);
					break;
				}
			}
			child->AddComponentBack<Component>(base);
		}
		child->transform = nullptr;
		objectName = GetNewName(gameObject->child[i]->GetObjectName());
		child->SetObjectName(objectName);
		obj->child.push_back(child.get());
		obj->childName.push_back(child->GetObjectName());
		child->parent = obj;
		child->parentName = obj->GetObjectName();
		child->Initialize();
		objectList.push_back(std::move(child));
	}
	obj->Initialize();
	obj->transform->position = position;
	return obj;
}

Object3D* IFE::ObjectManager::Instantiate(Object3D* gameObject)
{
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	//*ptr = *gameObject;
	ptr->isActive = gameObject->isActive;
	ptr->deleteFlag = gameObject->deleteFlag;
	ptr->DrawFlag = gameObject->DrawFlag;
	std::vector<string>s = gameObject->GetAllComponentName();
	for (int32_t i = 0; i < s.size(); i++)
	{
		if (s[i].find("_Model") != std::string::npos)
		{
			ptr->SetModel(ModelManager::Instance()->GetModel(s[i]));
			continue;
		}
		Component* base;
		base = StringToComponent(s[i]);
		for (auto& itr : gameObject->componentList)
		{
			if (itr->componentName == s[i])
			{
				base->Copy(itr);
				break;
			}
		}
		ptr->AddComponentBack<Component>(base);
	}
	ptr->transform = nullptr;
	string objectName = GetNewName(gameObject->GetObjectName());
	objectList.push_back(std::move(ptr));
	Object3D* obj = objectList.back().get();
	obj->child.clear();
	obj->childName.clear();
	obj->SetObjectName(objectName);
	for (int32_t i = 0; i < gameObject->child.size(); i++)
	{
		std::unique_ptr<Object3D> child = make_unique<Object3D>();
		child->isActive = gameObject->child[i]->isActive;
		child->deleteFlag = gameObject->child[i]->deleteFlag;
		child->DrawFlag = gameObject->child[i]->DrawFlag;
		std::vector<string>cs = gameObject->child[i]->GetAllComponentName();
		for (int32_t j = 0; j < cs.size(); j++)
		{
			if (cs[j].find("_Model") != std::string::npos)
			{
				child->SetModel(ModelManager::Instance()->GetModel(cs[j]));
				continue;
			}
			Component* base;
			base = StringToComponent(cs[j]);
			for (auto& itr : gameObject->child[i]->componentList)
			{
				if (itr->componentName == cs[i])
				{
					base->Copy(itr);
					break;
				}
			}
			child->AddComponentBack<Component>(base);
		}
		child->transform = nullptr;
		objectName = GetNewName(gameObject->child[i]->GetObjectName());
		child->SetObjectName(objectName);
		obj->child.push_back(child.get());
		obj->childName.push_back(child->GetObjectName());
		child->parent = obj;
		child->parentName = obj->GetObjectName();
		child->Initialize();
		objectList.push_back(std::move(child));
	}
	obj->Initialize();
	return obj;
}

Object3D* IFE::ObjectManager::GetPrefab(std::string name)
{
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		if (itr->GetObjectName() == name)return itr.get();
	}
	return nullptr;
}

void IFE::ObjectManager::SetModel(std::string objectName, Component* model)
{
	for (unique_ptr<Object3D>& itr : objectList)
	{
		if (itr->GetObjectName() == objectName)
		{
			itr->SetModel(model);
			return;
		}
	}
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		if (itr->GetObjectName() == objectName)
		{
			itr->SetModel(model);
			return;
		}
	}
}

std::list<std::unique_ptr<Object3D>>* IFE::ObjectManager::GetObjList()
{
	return &objectList;
}

void IFE::ObjectManager::Reset()
{
	objectList.clear();
	prefabList.clear();
}

bool IFE::ObjectManager::SearchName(std::string name)
{
	for (unique_ptr<Object3D>& itr : objectList)
	{
		if (itr->GetObjectName() == name)return true;
	}
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		if (itr->GetObjectName() == name)return true;
	}
	return false;
}

#ifdef _DEBUG
#include "imgui.h"
void IFE::ObjectManager::DebugGUI()
{
	static bool add = false;
	static bool fdelete = false;
	static bool prefab = false;
	static bool moveF = false;
	ImguiManager* im = ImguiManager::Instance();
	im->ObjectManagerGUI(&add, &fdelete, &prefab, &moveF);
	if (prefab)
	{
		im->TextGUI("PrefabManager");
	}
	else
	{
		im->TextGUI("ObjectManager");
	}
	if (add)
	{
		std::function<string(void)> func = []()
		{
			return ModelManager::Instance()->GetModelNameGUI();
		};
		static string name;
		static string model;
		static bool pre = false;
		if (pre)
		{
			im->CheckBoxGUI(&pre, "add new object");
			static int32_t num = 0;
			std::function<void(void)>prefunc = [&]()
			{
				static char n[256];
				ImGui::InputText("Add Object Name", n, sizeof(n));
				name = n;
				int32_t i = 0;
				for (unique_ptr<Object3D>& itr : prefabList)
				{
					ImGui::RadioButton(itr->GetObjectName().c_str(), &num, i);
					i++;
				}
				if (ImGui::Button("Add Object"))
				{
					string s;
					int32_t j = 0;
					for (unique_ptr<Object3D>& itr : prefabList)
					{
						if (num == j)
						{
							s = itr->GetObjectName();
							break;
						}
						j++;
					}
					if (s != "")AddFromPrefab(name, s);
					else
					{
						im->TextGUI("prefab not found");
					}
				}
			};
			if (prefabList.size() != 0)im->CollapsingHeaderGUI("Add from Prefab", prefunc);
			else
			{
				im->TextGUI("prefab not found");
			}
		}
		else
		{
			im->CheckBoxGUI(&pre, "add from prefab");
			if (im->ObjectAddGUI(&name, &model, func))
			{
				if (SearchName(name))
				{
					im->TextGUI("error : an object with the same name exists");
				}
				else
				{
					AddInitialize(name, ModelManager::Instance()->GetModel(model));
					im->TextGUI("success");
				}
			}
		}
	}
	string str;
	if (!prefab)
	{
		for (unique_ptr<Object3D>& itr : objectList)
		{
			string s;
			itr->DebugGUI(fdelete, moveF, &s);
			if (s != "")
			{
				str = s;
			}
		}

		im->EndGUI();
		for (unique_ptr<Object3D>& itr : objectList)
		{
			itr->ComponentGUI();
		}

		for (unique_ptr<Object3D>& itr : objectList)
		{
			if (itr->GetObjectName() == str)
			{
				prefabList.push_back(std::move(itr));
				objectList.remove(itr);
				break;
			}
		}
	}
	else
	{
		for (unique_ptr<Object3D>& itr : prefabList)
		{
			string s;
			itr->DebugGUI(fdelete, moveF, &s);
			if (s != "")
			{
				str = s;
			}
		}

		im->EndGUI();
		for (unique_ptr<Object3D>& itr : prefabList)
		{
			itr->ComponentGUI();
		}
		for (unique_ptr<Object3D>& itr : prefabList)
		{
			if (itr->GetObjectName() == str)
			{
				objectList.push_back(std::move(itr));
				prefabList.remove(itr);
				break;
			}
		}
	}

}

void IFE::ObjectManager::OutputScene()
{
	JsonManager* jm = JsonManager::Instance();
	int32_t i = 0;
	for (unique_ptr<Object3D>& itr : objectList)
	{
		nlohmann::json& js = jm->GetJsonData();
		js[i] = itr->GetObjectName();
		i++;
	}
	jm->OutputAndMakeDirectry("ObjectManager", "Object");
	for (unique_ptr<Object3D>& itr : objectList)
	{
		nlohmann::json& js = jm->GetJsonData();
		vector<string>names = itr->GetAllComponentName();
		for (int32_t j = 0; j < names.size(); j++)
		{
			js["com"][j] = names[j];
		}
		js["parent"] = itr->parentName;
		for (int32_t j = 0; j < itr->childName.size(); j++)
		{
			js["child"][j] = itr->childName[j];
		}
		jm->OutputAndMakeDirectry(itr->GetObjectName(), "Object");
	}
	i = 0;
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		nlohmann::json& js = jm->GetJsonData();
		js[i] = itr->GetObjectName();
		i++;
	}
	jm->OutputAndMakeDirectry("PrefabManager", "Object");
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		nlohmann::json& js = jm->GetJsonData();
		vector<string>names = itr->GetAllComponentName();
		for (int32_t j = 0; j < names.size(); j++)
		{
			js["com"][j] = names[j];
		}
		js["parent"] = itr->parentName;
		for (int32_t j = 0; j < itr->childName.size(); j++)
		{
			js["child"][j] = itr->childName[j];
		}
		string s = "P" + itr->GetObjectName();
		jm->OutputAndMakeDirectry(s, "Object");
	}
	for (unique_ptr<Object3D>& itr : objectList)
	{
		itr->OutputScene();
	}
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		itr->OutputScene();
	}
}
void IFE::ObjectManager::DebugUpdate()
{
	objectList.remove_if([](unique_ptr<Object3D>& obj) {return obj->deleteFlag; });
	prefabList.remove_if([](unique_ptr<Object3D>& obj) {return obj->deleteFlag; });
	for (unique_ptr<Object3D>& itr : objectList)
	{
		itr->DebugUpdate();
	}
}
#endif
void IFE::ObjectManager::LoadingScene()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("Object/ObjectManager");
	nlohmann::json js = jm->GetJsonData();
	for (auto itr : js)
	{
		Add(itr);
	}

	jm->JsonReset();
	for (unique_ptr<Object3D>& itr : objectList)
	{
		string s = "Object/" + itr->GetObjectName();
		jm->Input(s);
		nlohmann::json js2 = jm->GetJsonData();
		vector<string>names;
		for (auto itr2 : js2["com"])
		{
			names.push_back(itr2);
		}
		itr->parentName = js2["parent"];
		for (auto itr2 : js2["child"])
		{
			if (itr2 == "")continue;
			itr->childName.push_back(itr2);
		}
		jm->JsonReset();
		for (int32_t i = 0; i < names.size(); i++)
		{
			//‚ ‚Æ‚Å•Ê‚Ìcpp‚É
			Component* base = nullptr;

			if (names[i].find("_Model") != std::string::npos)
			{
				itr->SetModel(ModelManager::Instance()->GetModel(names[i]));
				continue;
			}
			else
			{
				base = StringToComponent(names[i]);
				base->LoadingScene(itr->GetObjectName(), names[i]);
			}
			itr->SetComponent(base);
		}
		itr->Initialize();
	}
	jm->Input("Object/PrefabManager");
	js = jm->GetJsonData();
	for (auto itr : js)
	{
		AddPrefab(itr);
	}

	jm->JsonReset();
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		string s = "Object/P" + itr->GetObjectName();
		jm->Input(s);
		nlohmann::json js2 = jm->GetJsonData();
		vector<string>names;
		for (auto itr2 : js2["com"])
		{
			names.push_back(itr2);
		}
		itr->parentName = js2["parent"];
		for (auto itr2 : js2["child"])
		{
			if (itr2 == "")continue;
			itr->childName.push_back(itr2);
		}
		jm->JsonReset();
		for (int32_t i = 0; i < names.size(); i++)
		{
			//‚ ‚Æ‚Å•Ê‚Ìcpp‚É
			Component* base = nullptr;

			if (names[i].find("_Model") != std::string::npos)
			{
				itr->SetModel(ModelManager::Instance()->GetModel(names[i]));
				continue;
			}
			else
			{
				base = StringToComponent(names[i]);
				base->LoadingScene(itr->GetObjectName(), names[i]);
			}
			itr->SetComponent(base);
		}
		itr->Initialize();
	}
	for (unique_ptr<Object3D>& itr : objectList)
	{
		itr->LoadChild();
	}
	for (unique_ptr<Object3D>& itr : prefabList)
	{
		itr->LoadChild();
	}
}

std::string IFE::ObjectManager::GetNewName(std::string tag)
{
	int32_t i = 0;
	string r;
	while (true)
	{
		string s = tag + to_string(i);
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

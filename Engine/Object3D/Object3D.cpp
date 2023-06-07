#include "Object3D.h"
#include "ObjectManager.h"
#include "PrimitiveModel.h"
#include "GraphicsPipeline.h"
#include "TextureManager.h"
#include "Material.h"
#include "Texture.h"
#include "TransferGeometryBuffer.h"
#include "Transform.h"
#include "FBXModel.h"
#include "Collider.h"
#include "ImguiManager.h"

using namespace IFE;
using namespace std;

IFE::Object3D::Object3D()
{
	SetObjectPtr(this);
}

void IFE::Object3D::OBJInitialize()
{
	AddComponent<Material>();
	AddComponent<TransferGeometryBuffer>();
	AddComponent<Transform>();
	ComponentManager::Initialize();
	Material* m = GetComponent<Material>();
	m->SetDefaultParameter();
}

void IFE::Object3D::Initialize()
{
	ComponentManager::Initialize();
	transform = GetComponent<Transform>();
}

void IFE::Object3D::SetModel(Component* model)
{
	RemoveComponent<PrimitiveModel>();
	RemoveComponent<FBXModel>();
	AddComponentBack<Component>(model, nullptr);
}

void IFE::Object3D::SetComponent(Component* component)
{
	AddComponentBack<Component>(component);
}
void IFE::Object3D::SetComponentFront(Component* component)
{
	AddComponent<Component>(component);
}

void IFE::Object3D::OBJUpdate()
{
	if (!isActive)return;
	ComponentManager::Update();
	childCount = (int32_t)child.size();
	if (deleteFlag == true)
	{
		for (int32_t i = 0; i < child.size(); i++)
		{
			child[i]->deleteFlag = true;
		}
	}
}

void IFE::Object3D::DrawBefore(D3D_PRIMITIVE_TOPOLOGY topology)
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	commandList->IASetPrimitiveTopology(topology);
}

void IFE::Object3D::Draw()
{
	if (!isActive)return;
	if (!DrawFlag)return;
	ComponentManager::Draw();
}

IFE::Object3D::~Object3D()
{
	ComponentManager::Finalize();
}

std::string IFE::Object3D::GetObjectName()
{
	return objectName;
}

void IFE::Object3D::SetObjectName(std::string n)
{
	objectName = n;
}

vector<string> IFE::Object3D::GetAllComponentName()
{
	return ComponentManager::GetAllComponentName();
}

void IFE::Object3D::SetActive(bool value)
{
	isActive = value;
}

Object3D* IFE::Object3D::GetChild(int32_t index)
{
	return child[index];
}

void IFE::Object3D::DeleteParent()
{
	for (int32_t i = 0; i < parent->childName.size(); i++)
	{
		if (parent->childName[i] == objectName)
		{
			parent->childName.erase(childName.begin() + i);
			parent->child.erase(child.begin() + i);
			break;
		}
	}
	parent = nullptr;
	parentName = "";
}

void IFE::Object3D::DeleteChild(int32_t index)
{
	Object3D* obj = GetChild(index);
	obj->parent = nullptr;
	obj->parentName = "";
	childName.erase(childName.begin() + index);
	child.erase(child.begin() + index);
}

void IFE::Object3D::DeleteChild(std::string c)
{
	for (int32_t i = 0; i < childName.size(); i++)
	{
		if (childName[i] == c)
		{
			DeleteChild(i);
			return;
		}
	}
}

void IFE::Object3D::DeleteChildAll()
{
	for (int32_t i = 0; i < parent->child.size(); i++)
	{
		child[i]->parent = nullptr;
		child[i]->parentName = "";
	}
	child.clear();
}
//void IFE::Object3D::OnColliderHit(ADXCollider* myCol, ADXCollider* col)
//{
//	ComponentManager::OnColliderHit(myCol, col);
//}

#ifdef _DEBUG
void IFE::Object3D::DebugGUI(bool fdelete, bool fmove, std::string* str)
{
	bool m = false;
	if (ImguiManager::Instance()->ObjectGUI(objectName, fdelete, fmove, &m, this))
	{
		deleteFlag = true;
	}
	if (m)
	{
		*str = objectName;
	}
}

void IFE::Object3D::ComponentGUI()
{
	std::function<void(Component*)> addFunc = [&](Component* com)
	{
		SetComponentFront(com);
	};
	//std::function<void(Component*)> modelFunc = [&](Component* com)
	//{
	//	SetModel(com);
	//};
	std::function<void(void)>f = [&]()
	{
		ComponentManager::DebugGUI();
	};
	ImguiManager::Instance()->ComponentGUI(objectName, f, addFunc/*, modelFunc*/);
}

void IFE::Object3D::OutputScene(bool flag)
{
	if (flag)
	{
		string s = "P" + objectName;
		ComponentManager::OutputScene(s);
	}
	else
	{
		ComponentManager::OutputScene(objectName);
	}
}
void IFE::Object3D::DebugUpdate()
{
	ComponentManager::DebugUpdate();
	childCount = (int32_t)child.size();
	if (deleteFlag == true)
	{
		for (int32_t i = 0; i < child.size(); i++)
		{
			child[i]->deleteFlag = true;
		}
	}
}

#endif
//void IFE::Object3D::LoadingScene()
//{
//	ComponentManager::LoadingScene(objectName,);
//}

void IFE::Object3D::LoadChild()
{
	if (parentName != "")
	{
		parent = ObjectManager::Instance()->GetObjectPtr(parentName);
	}
	for (int32_t i = 0; i < childName.size(); i++)
	{
		child.push_back(ObjectManager::Instance()->GetObjectPtr(childName[i]));
	}
}

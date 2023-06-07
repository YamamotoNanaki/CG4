#ifdef _DEBUG
#include "ImguiManager.h"
#include "ObjectManager.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include <Windows.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include "GraphicsAPI.h"
#include "WindowsAPI.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ComponentManager.h"
#include "Texture.h"
#include "ComponentHelp.h"

using namespace IFE;

std::string ImguiManager::openComponentName;

ImguiManager* IFE::ImguiManager::Instance()
{
	static ImguiManager inst;
	return &inst;
}

void IFE::ImguiManager::Initialize()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(*WindowsAPI::Instance()->GetHWnd());
	ID3D12DescriptorHeap* SrvDescHeap = TextureManager::Instance()->GetDescriptorHeap();
	ImGui_ImplDX12_Init(GraphicsAPI::Instance()->GetDevice(), NUM_FRAMES_IN_FLIGHT,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, SrvDescHeap,
		SrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
		SrvDescHeap->GetGPUDescriptorHandleForHeapStart());
}

void IFE::ImguiManager::StartNewFrame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void IFE::ImguiManager::Draw()
{
	ImGui::Render();
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	ID3D12DescriptorHeap* heaps[] = { TextureManager::Instance()->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void IFE::ImguiManager::Demo()
{
	ImGui::ShowDemoWindow();
}

void IFE::ImguiManager::EndGUI()
{
	ImGui::End();
}

bool IFE::ImguiManager::ButtonGUI(const std::string& buttonName)
{
	return ImGui::Button(buttonName.c_str());
}

void IFE::ImguiManager::RadioButtonGUI(const std::string& buttonName, int32_t* num, const  int32_t& buttonNum)
{
	ImGui::RadioButton(buttonName.c_str(), num, buttonNum);
}

void IFE::ImguiManager::ObjectManagerGUI(bool* add, bool* fdelete, bool* prefab, bool* fmove)
{
	ImGui::Begin("ObjectManager", (bool*)false, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("add"))
		{
			ImGui::MenuItem("Add", "", add);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("delete"))
		{
			ImGui::MenuItem("delete", "", fdelete);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("move"))
		{
			ImGui::MenuItem("Prefab and Object Switching", "", fmove);
			ImGui::EndMenu();
		}
		if (*prefab)
		{
			if (ImGui::BeginMenu("ObjectManager"))
			{
				ImGui::MenuItem("ObjectManager", "", prefab);
				ImGui::EndMenu();
			}
		}
		else
		{
			if (ImGui::BeginMenu("PrefabManager"))
			{
				ImGui::MenuItem("PrefabManager", "", prefab);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}
}
static bool componentDeleteFlag = false;

void IFE::ImguiManager::ComponentGUI(const std::string& objectName, const std::function<void(void)>& ComponentFunc, const std::function<void(Component*)>& addFunc/*, const  std::function<void(Component*)>& modelFunc*/)
{
	static bool add = false;
	static bool cm = false;
	if (objectName == openComponentName)
	{
		ImGui::Begin("Component List", (bool*)false, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("add"))
			{
				ImGui::MenuItem("Add", "", &add);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("delete"))
			{
				ImGui::MenuItem("Delete", "", &componentDeleteFlag);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("change model"))
			{
				ImGui::MenuItem("Change Model", "", &cm);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (add)
		{
			if (ImGui::CollapsingHeader("add"))
			{
				static char name[256];
				ImGui::InputText("add Component", name, sizeof(name));
				if (ImGui::Button("Add"))
				{
					Component* tmp = std::move(StringToComponent(name));
					if (tmp != nullptr)
					{
						addFunc(tmp);
					}
				}
			}
		}
		//if (cm)
		//{
		//	ModelManager::Instance()->GetModelNameGUI();
		//	if (ImGui::Button("Change"))
		//	{
		//		modelFunc();
		//	}
		//}
		ImGui::Text(objectName.c_str());
		ComponentFunc();
		ImGui::End();
	}
}

void IFE::ImguiManager::ComponentGUI2D(const std::string& objectName, const std::function<void(void)>& ComponentFunc, const std::function<void(Component*)>& addFunc/*, const std::function<void(std::string)>& texFunc*/)
{
	static bool add = false;
	static bool cm = false;
	if (objectName == openComponentName)
	{
		ImGui::Begin("Component List", (bool*)false, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("add"))
			{
				ImGui::MenuItem("Add", "", &add);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("delete"))
			{
				ImGui::MenuItem("Delete", "", &componentDeleteFlag);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("change model"))
			{
				ImGui::MenuItem("Change Model", "", &cm);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (add)
		{
			if (ImGui::CollapsingHeader("add"))
			{
				static char name[256];
				ImGui::InputText("add Component", name, sizeof(name));
				if (ImGui::Button("Add"))
				{
					Component* tmp = std::move(StringToComponent(name));
					if (tmp != nullptr)
					{
						addFunc(tmp);
					}
				}
			}
		}
		//if (cm)
		//{
		//	ModelManager::Instance()->GetModelNameGUI();
		//	if (ImGui::Button("Change"))
		//	{
		//		modelFunc();
		//	}
		//}
		ImGui::Text(objectName.c_str());
		ComponentFunc();
		ImGui::End();
	}
}

bool IFE::ImguiManager::ObjectGUI(const std::string& name, const bool& flagdelete, const  bool& m, bool* moveFlag, Object3D* obj)
{
	if (ImGui::TreeNode(name.c_str()))
	{
		if (ImGui::Button("Open Component"))
		{
			openComponentName = name;
		}
		if (flagdelete)
		{
			if (ImGui::Button("Object Delete"))
			{
				ImGui::TreePop();
				return true;
			}
		}
		if (m)
		{
			if (ImGui::Button("Prefab and Object Switching"))
			{
				*moveFlag = true;
			}
		}
		if (obj->child.size() != 0)
		{
			if (ImGui::TreeNode("child"))
			{
				for (int32_t i = 0; i < obj->childName.size(); i++)
				{
					ImGui::Text(obj->childName[i].c_str());
					std::string s = "delete " + obj->childName[i];
					if (ImGui::Button(s.c_str()))
					{
						obj->DeleteChild(i);
					}
				}
				ImGui::TreePop();
			}
		}
		if (obj->parent != nullptr)
		{
			if (ImGui::TreeNode("parent"))
			{
				ImGui::Text(obj->parent->GetObjectName().c_str());
				ImGui::TreePop();
			}
		}
		if (ImGui::TreeNode("Set Child"))
		{
			static char n[256];
			ImGui::InputText("child Name", n, sizeof(n));
			if (ImGui::Button("Set"))
			{
				Object3D* ptr = ObjectManager::Instance()->GetObjectPtr(n);
				if (ptr != nullptr)
				{
					obj->child.push_back(ptr);
					obj->childName.push_back(ptr->GetObjectName());
					ptr->parent = obj;
					ptr->parentName = obj->GetObjectName();
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	return false;
}

bool IFE::ImguiManager::SpriteGUI(const std::string& name, const bool& flagdelete, const bool& m, bool* moveFlag)
{
	if (ImGui::TreeNode(name.c_str()))
	{
		if (ImGui::Button("Open Component"))
		{
			openComponentName = name;
		}
		if (flagdelete)
		{
			if (ImGui::Button("Object Delete"))
			{
				ImGui::TreePop();
				return true;
			}
		}
		ImGui::TreePop();
	}
	if (m)
	{
		if (ImGui::Button("Prefab and Object Switching"))
		{
			*moveFlag = true;
		}
	}
	return false;
}

bool IFE::ImguiManager::EmitterGUI(const std::string& name, const bool& flagdelete)
{
	if (ImGui::TreeNode(name.c_str()))
	{
		if (ImGui::Button("Open Component"))
		{
			openComponentName = name;
		}
		if (flagdelete)
		{
			if (ImGui::Button("Object Delete"))
			{
				ImGui::TreePop();
				return true;
			}
		}
		ImGui::TreePop();
	}
	return false;
}

bool IFE::ImguiManager::ObjectAddGUI(std::string* newObjectName, std::string* modelName, const std::function<std::string(void)>& guiFunc)
{
	if (ImGui::CollapsingHeader("New Object"))
	{
		static char name[256];
		ImGui::InputText("New Object Name", name, sizeof(name));
		*newObjectName = name;
		*modelName = guiFunc();
		return ImGui::Button("Add Object");
	}
	return false;
}

void IFE::ImguiManager::NewGUI(const std::string& guiName, const int32_t& flag)
{
	ImGui::Begin(guiName.c_str(), (bool*)false, (ImGuiWindowFlags)flag);
}

void IFE::ImguiManager::ModelManagerGUI(bool* add, bool* fdelete)
{
	ImGui::Begin("ModelManager", (bool*)false, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("add"))
		{
			ImGui::MenuItem("Add", "", add);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("delete"))
		{
			ImGui::MenuItem("delete", "", fdelete);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

bool IFE::ImguiManager::ModelAddGUI(std::string* newObjectName, std::string* newFilename, uint16_t* settings, bool* smooth)
{
	if (ImGui::CollapsingHeader("New Model"))
	{
		static char name[256];
		ImGui::InputText("New Model Name", name, sizeof(name));
		*newObjectName = name;

		ImGui::Checkbox("smooth", smooth);

		int32_t num = (int32_t)*settings;
		ImGui::RadioButton("Load .obj", &num, (int32_t)AddModelSettings::LoadOBJ);
		ImGui::RadioButton("Load .gltf", &num, (int32_t)AddModelSettings::LoadGLTF);
		ImGui::RadioButton("Primitiv Cube", &num, (int32_t)AddModelSettings::CreateCube);
		ImGui::RadioButton("Primitiv Square", &num, (int32_t)AddModelSettings::CreateSquare);
		ImGui::RadioButton("Primitiv Triangle", &num, (int32_t)AddModelSettings::CreateTriangle);
		ImGui::RadioButton("Primitiv Circle", &num, (int32_t)AddModelSettings::CreateCircle);
		ImGui::RadioButton("Primitiv Sphere", &num, (int32_t)AddModelSettings::CreateSphere);

		*settings = (uint16_t)num;

		if (num < (int32_t)AddModelSettings::CreateCube)
		{
			static char filename[256];
			ImGui::InputText("File Name", filename, sizeof(filename));
			*newFilename = filename;
		}
		return ImGui::Button("Add Model");
	}
	return false;
}

int32_t IFE::ImguiManager::GetModelNameGUI(const std::vector<std::string>& names)
{
	static int32_t returnnum = 0;
	for (int32_t i = 0; i < names.size(); i++)
	{
		ImGui::RadioButton(names[i].c_str(), &returnnum, i);
	}
	return returnnum;
}

void IFE::ImguiManager::ModelGUI(const std::string& modelName)
{
	if (ImGui::CollapsingHeader(modelName.c_str()))
	{
	}
}

void IFE::ImguiManager::ComponentGUI(const std::function<void(void)>& guiFunc, const std::function<void(void)>& deleteFunc, const std::string& componentName)
{
	if (ImGui::CollapsingHeader(componentName.c_str()))
	{
		if (componentDeleteFlag)
		{
			if (ImGui::Button("Remove"))
			{
				deleteFunc();
			}
		}
		guiFunc();
	}
}

void IFE::ImguiManager::ChangeTextureGUI(const std::function<void(std::string)>& guiFunc)
{
	if (ImGui::CollapsingHeader("change texture"))
	{
		static char texname[256];
		ImGui::InputText("texture name", texname, sizeof(texname));
		if (ImGui::Button("change"))
		{
			static std::string s = texname;
			guiFunc(s);
		}
	}
}

void IFE::ImguiManager::TextureGUI(bool* load, bool* search)
{
	ImGui::Begin("TextureManager", (bool*)false, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("load"))
		{
			ImGui::MenuItem("load", "", load);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("search"))
		{
			ImGui::MenuItem("search", "", search);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void IFE::ImguiManager::TextureLoadGUI(const std::function<void(std::string)>& guiFunc)
{
	if (ImGui::CollapsingHeader("Texture Load"))
	{
		static char filename[256];
		ImGui::InputText("filename", filename, sizeof(filename));
		if (ImGui::Button("Load"))
		{
			std::string f = filename;
			guiFunc(f);
		}
	}
}

void IFE::ImguiManager::ShowTextureGUI(const std::array<Texture, 1024>& tex)
{
	if (ImGui::CollapsingHeader("All Textures"))
	{
		Texture* t[5];
		int32_t j = 0;
		for (int32_t i = 1; i < 1024; i++)
		{
			if (!tex[i].free)continue;
			t[j] = tex[i].GetPtr();
			j++;
			if (j % 5 == 4)
			{
				for (int32_t k = 0; k < 5; k++)
				{
					std::string s = std::to_string(k) + ". " + t[k]->texName;
					ImGui::Text(s.c_str());
					if (k != 4)ImGui::SameLine();
				}
				for (int32_t k = 0; k < 5; k++)
				{
					ImGui::Image((ImTextureID)t[k]->GPUHandle.ptr, { 96,96 });
					if (k != 4)ImGui::SameLine();
				}
			}
		}

		for (int32_t k = 0; k < j; k++)
		{
			std::string s = std::to_string(k) + ". " + t[k]->texName;
			ImGui::Text(s.c_str());
			if (k != j - 1)ImGui::SameLine();
		}
		for (int32_t k = 0; k < j; k++)
		{
			ImGui::Image((ImTextureID)t[k]->GPUHandle.ptr, { 96,96 });
			if (k != j - 1)ImGui::SameLine();
		}
	}
}

void IFE::ImguiManager::SearchTextureGUI(const std::array<Texture, 1024>& texture)
{
	if (ImGui::CollapsingHeader("search"))
	{
		static bool flag = false;
		static std::string SearchName;
		static char s[256];
		ImGui::InputText("search texture name", s, sizeof(s));
		if (ImGui::Button("search texture"))
		{
			if (!flag)flag = true;
			SearchName = s;
		}
		if (flag)
		{
			for (int32_t i = 1; i < 1024; i++)
			{
				if (!texture[i].free)continue;
				if (texture[i].texName == SearchName)
				{
					ImGui::Image((ImTextureID)texture[i].GPUHandle.ptr, { 96,96 });
					break;
				}
			}
		}
	}
}

void IFE::ImguiManager::TextGUI(const std::string& text)
{
	ImGui::Text(text.c_str());
}

void IFE::ImguiManager::DragIntGUI(int32_t* i, const std::string& label, const float& speed, const int32_t& min, const int32_t& max)
{
	ImGui::DragInt(label.c_str(), i, speed, min, max);
}

void IFE::ImguiManager::DragFloatGUI(float* f, const std::string& label, const float& speed, const  float& min, const float& max)
{
	ImGui::DragFloat(label.c_str(), f, speed, min, max);
}

void IFE::ImguiManager::DragFloat2GUI(Float2* f, const std::string& label, const float& speed, const float& min, const float& max)
{
	float a[2] = { f->x,f->y };
	ImGui::DragFloat2(label.c_str(), a, speed, min, max);
	f->Set(a[0], a[1]);
}

void IFE::ImguiManager::DragFloat3GUI(Float3* f, const std::string& label, const float& speed, const float& min, const float& max)
{
	float a[3] = { f->x,f->y,f->z };
	ImGui::DragFloat3(label.c_str(), a, speed, min, max);
	f->Set(a[0], a[1], a[2]);
}

void IFE::ImguiManager::DragFloat4GUI(Float4* f, const std::string& label, const float& speed, const float& min, const float& max)
{
	float a[4] = { f->x,f->y,f->z,f->w };
	ImGui::DragFloat4(label.c_str(), a, speed, min, max);
	f->Set(a[0], a[1], a[2], a[3]);
}

void IFE::ImguiManager::ColorEdit4GUI(Float4* color, const std::string& label)
{
	float c[4] = { color->x,color->y,color->z,color->w };
	ImGui::ColorEdit4(label.c_str(), c);
	color->Set(c[0], c[1], c[2], c[3]);
}

void IFE::ImguiManager::CheckBoxGUI(bool* flag, const std::string& label)
{
	ImGui::Checkbox(label.c_str(), flag);
}

void IFE::ImguiManager::CollapsingHeaderGUI(const std::string& label, const std::function<void(void)>& guiFunc)
{
	if (ImGui::CollapsingHeader(label.c_str()))
	{
		guiFunc();
	}
}

bool IFE::ImguiManager::NewTreeNode(const std::string& label)
{
	return ImGui::TreeNode(label.c_str());
}

void IFE::ImguiManager::EndTreeNode()
{
	ImGui::TreePop();
}

Object3D* IFE::ImguiManager::SetObject(Object3D* obj, const std::string& label)
{
	static bool error = false;
	if (ImGui::TreeNode(label.c_str()))
	{
		if (obj != nullptr)ImGui::Text(obj->GetObjectName().c_str());
		else ImGui::Text("nullptr");
		if (ImGui::TreeNode("Change Ptr"))
		{
			static char name[256];
			ImGui::InputText("Object Name", name, sizeof(name));
			if (error)ImGui::Text("error : return nullptr");

			if (ImGui::Button("Change"))
			{
				Object3D* ptr = ObjectManager::Instance()->GetObjectPtr(name);
				if (ptr != nullptr)
				{
					error = false;
					obj = ptr;
				}
				else error = true;
			}
			if (ImGui::Button("Change nullptr"))
			{
				obj = nullptr;
			}

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	return obj;
}



#endif
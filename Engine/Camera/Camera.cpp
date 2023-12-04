#include "Camera.h"
#include "Debug.h"
#include "WindowsAPI.h"
#include "ComponentHelp.h"
#include "Transform.h"
#include "Fog.h"

using namespace IFE;
using namespace std;

Camera::Camera()
{
	SetCameraPtr(this);
}

void IFE::Camera::CameraInitialize()
{
	AddComponent<TransformCamera>();
	AddComponent<Fog>();
	ComponentManager::Initialize();
	transform_ = GetComponent<TransformCamera>();
}

void IFE::Camera::CameraUpdate()
{
	projection_.Update();
	ComponentManager::Update();
}

View* IFE::Camera::GetView()
{
	return &view_;
}

Projection* IFE::Camera::GetProjection()
{
	return &projection_;
}

IFE::Camera::~Camera()
{
}

void IFE::Camera::Initialize()
{
	ComponentManager::Initialize();
	transform_ = GetComponent<TransformCamera>();
}

void IFE::Camera::SetComponent(std::unique_ptr<Component> component)
{
	AddComponentBack<Component>(std::move(component));
}
void IFE::Camera::SetComponentFront(std::unique_ptr<Component> component)
{
	AddComponent<Component>(std::move(component));
}

#ifdef NDEBUG
#else
#include "ImguiManager.h"
#include "ImGui.h"
void IFE::Camera::DebugGUI(bool fdelete)
{
	auto imgui = ImguiManager::Instance();

	if (imgui->NewTreeNode(cameraName_))
	{
		if (imgui->ButtonGUI("Open Component"))
		{
			imgui->sOpenComponentName_ = cameraName_;
		}
		if (fdelete)
		{
			if (imgui->ButtonGUI("Camera Delete"))
			{
				imgui->EndTreeNode();
				deleteFlag_ = true;
				return;
			}
		}
		imgui->EndTreeNode();
	}
}

void IFE::Camera::DebugUpdate()
{
	projection_.Update();
	transform_->Update();
	GetComponent<Fog>()->Update();
}
#include "Input.h"

void IFE::Camera::DebugCameraUpdate()
{
	//Input* input = Input::Instance();
	//static bool flag = false;
	//if (input->GetMouseTrigger(Mouse::Middle))flag = !flag;
	//if (flag)
	//{
	//	Float2 move = input->GetMousePoint();
	//	bool dirty = false;
	//	float angleX = 0;
	//	float angleY = 0;

	//	if (input->GetMousePush(Mouse::Left))
	//	{
	//		float dy = move.x * scaleY;
	//		float dx = move.y * scaleX;

	//		angleX = -dx * M_PI;
	//		angleY = -dy * M_PI;
	//		dirty = true;
	//	}

	//	if (input->GetMousePush(Mouse::Right))
	//	{
	//		float dx = move.x / 20.0f;
	//		float dy = move.y / 20.0f;

	//		Vector3 move2 = { -dx, +dy, 0 };
	//		move2 = Vector3Transform(move2, matRot);

	//		Float3 eye = matView->eye;
	//		Float3 target = matView->target;

	//		eye.x += move2.x;
	//		eye.y += move2.y;
	//		eye.z += move2.z;

	//		target.x += move2.x;
	//		target.y += move2.y;
	//		target.z += move2.z;

	//		SetEye(eye);
	//		SetTarget(target);

	//		dirty = true;
	//	}

	//	if (move.z != 0)
	//	{
	//		distance -= move.z / 10.0f;
	//		distance = distance > 1.0f ? distance : 1.0f;
	//		dirty = true;
	//	}

	//	if (dirty) {
	//		Matrix newMatRot;
	//		newMatRot *= MatrixRotationX(-angleX);
	//		newMatRot *= MatrixRotationY(-angleY);

	//		matRot = newMatRot * matRot;


	//		Vector3 newTargetEye = { 0.0f, 0.0f, -distance };
	//		Vector3 newUp = { 0.0f, 1.0f, 0.0f };

	//		newTargetEye = Vector3Transform(newTargetEye, matRot);
	//		newUp = Vector3Transform(newUp, matRot);

	//		const Float3& newTarget = matView->target;
	//		SetEye({ newTarget.x + newTargetEye.x, newTarget.y + newTargetEye.y, newTarget.z + newTargetEye.z });
	//		SetUp({ newUp.x, newUp.y, newUp.z });
	//	}
	//}

	//cpos = SetVector3(matView->eye);
	//ctar = SetVector3(matView->target);
	//cupv = SetVector3(matView->up);
	//matView->Update();
	//projection_.Update();
	//transform_->Update();
	//GetComponent<Fog>()->Update();
}

void IFE::Camera::DebugCameraInitialize()
{
	ComponentManager::Initialize();
	transform_ = GetComponent<TransformCamera>();
}

void IFE::Camera::ComponentGUI()
{
	auto imgui = ImguiManager::Instance();

	if (cameraName_ == imgui->sOpenComponentName_)
	{
		imgui->NewGUI("Component List", (int32_t)ImGuiWindowFlags_MenuBar);
		if (imgui->NewMenuBar())
		{
			if (imgui->NewMenu("add"))
			{
				imgui->MenuItem("Add", &imgui->componentAddFlag_);
				imgui->EndMenu();
			}
			if (imgui->NewMenu("delete"))
			{
				imgui->MenuItem("Delete", &imgui->componentDeleteFlag_);
				imgui->EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (imgui->componentAddFlag_)
		{
			if (imgui->CollapsingHeaderGUI("add"))
			{
				std::string name;
				imgui->InputTextGUI("add Component", name);
				if (imgui->ButtonGUI("Add"))
				{
					auto tmp = std::unique_ptr<Component>(std::move(StringToComponent(name)));
					if (tmp != nullptr)
					{
						SetComponent(std::move(tmp));
					}
				}
			}
		}
		ComponentManager::DebugGUI();
		ImGui::End();
	}
}

void IFE::Camera::OutputScene(nlohmann::json& json)
{
	json["name"] = cameraName_;
	uint32_t i = 0;
	for (auto& com : componentList_)
	{
		json["component"][i] = com->componentName_;
		i++;
	}
	ComponentManager::OutputScene(json);
}
#endif

void IFE::Camera::LoadingScene(nlohmann::json& json)
{
	for (auto& com : json["component"])
	{
		ComponentManager::LoadingScene(json, com);
	}
}

#include "CameraManager.h"
#include "Debug.h"

using namespace IFE;
Camera* CameraManager::activCamera = nullptr;

IFE::CameraManager::~CameraManager()
{
	delete activCamera;
}

CameraManager* IFE::CameraManager::Instance()
{
	static CameraManager inst;
	return &inst;
}

void IFE::CameraManager::Initialize()
{
	activCamera = new Camera;
	activCamera->CameraInitialize();
}

void IFE::CameraManager::Update()
{
	activCamera->CameraUpdate();
}

#ifdef _DEBUG
#include "ImguiManager.h"
void IFE::CameraManager::DebugGUI()
{
	View* v = activCamera->GetView();
	ImguiManager* gui = ImguiManager::Instance();
	gui->NewGUI("Camera");
	gui->DragFloat3GUI(&v->eye, "eye");
	gui->DragFloat3GUI(&v->target, "target");
	gui->DragFloat3GUI(&v->up, "up");
	gui->EndGUI();
}
#endif

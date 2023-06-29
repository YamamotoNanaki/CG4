#include "CameraManager.h"
#include "Debug.h"

using namespace IFE;
Camera* CameraManager::sActivCamera_ = nullptr;

IFE::CameraManager::~CameraManager()
{
	cameraList.clear();
}

CameraManager* IFE::CameraManager::Instance()
{
	static CameraManager inst;
	return &inst;
}

void IFE::CameraManager::Initialize()
{
	cameraList.push_back(std::make_unique<Camera>());
	sActivCamera_ = cameraList.front().get();
	sActivCamera_->CameraInitialize();
}

void IFE::CameraManager::Update()
{
	sActivCamera_->CameraUpdate();
}

#ifdef _DEBUG
#include "ImguiManager.h"
void IFE::CameraManager::DebugGUI()
{
	View* v = sActivCamera_->GetView();
	ImguiManager* gui = ImguiManager::Instance();
	gui->NewGUI("Camera");
	gui->DragFloat3GUI(&v->eye_, "eye");
	gui->DragFloat3GUI(&v->target_, "target");
	gui->DragFloat3GUI(&v->up_, "up");
	gui->EndGUI();
}
#endif

#include "CameraManager.h"
#include "WindowsAPI.h"
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

	auto mat2d = MatrixOrthoGraphicProjection((float)0, (float)WindowsAPI::Instance()->winWidth_, (float)0, (float)WindowsAPI::Instance()->winHeight_, (float)0, (float)1);
	auto map = camera2D_.GetCBMapObject();
	map->pro = mat2d;
	camera2D_.SetConstBuffView(1);
}

void IFE::CameraManager::Update()
{
	sActivCamera_->CameraUpdate();
}

void IFE::CameraManager::Draw()
{
	sActivCamera_->Draw();
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

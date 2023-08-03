#include "CameraManager.h"
#include "Debug.h"

using namespace IFE;
Camera* CameraManager::sActivCamera_ = nullptr;

IFE::CameraManager::~CameraManager()
{
	cameraList_.clear();
}

CameraManager* IFE::CameraManager::Instance()
{
	static CameraManager inst;
	return &inst;
}

void IFE::CameraManager::Initialize()
{
	auto c = new Camera;
	cameraList_.push_back(std::unique_ptr<Camera>(c));
	sActivCamera_ = cameraList_.front().get();
	sActivCamera_->CameraInitialize();
}

void IFE::CameraManager::LoadingScene()
{
	sActivCamera_->GetView()->Initialze({ 0,10,-50 }, { 0,10,1 }, { 0,1,0 });
	//cameraList_.clear();
	//Initialize();
}

void IFE::CameraManager::Update()
{
	sActivCamera_->CameraUpdate();
}

#ifdef NDEBUG
#else
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

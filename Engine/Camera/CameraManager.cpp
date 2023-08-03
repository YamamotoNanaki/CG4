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
	c->cameraName_ = "initCamera";
	cameraList_.push_back(std::unique_ptr<Camera>(c));
	sActivCamera_ = cameraList_.front().get();
	sActivCamera_->CameraInitialize();
}

void IFE::CameraManager::Update()
{
	cameraList_.remove_if([](std::unique_ptr<Camera>& camera) {return camera->deleteFlag_; });
	if (!sActivCamera_)sActivCamera_ = cameraList_.front().get();
	sActivCamera_->CameraUpdate();
}

void IFE::CameraManager::Reset()
{
	cameraList_.clear();
}

Camera* IFE::CameraManager::AddInitialize(const std::string& cameraName)
{
	std::unique_ptr<Camera> ptr = std::make_unique<Camera>();
	ptr->cameraName_ = cameraName;
	ptr->CameraInitialize();
	cameraList_.push_back(std::move(ptr));
	return cameraList_.back().get();
}

bool IFE::CameraManager::SearchName(const std::string& name)
{
	for (auto& itr : cameraList_)
	{
		if (itr->cameraName_ == name)return true;
	}
	return false;
}

Camera* IFE::CameraManager::Add(const std::string& cameraName)
{
	std::unique_ptr<Camera> ptr = std::make_unique<Camera>();
	ptr->cameraName_ = cameraName;
	cameraList_.push_back(std::move(ptr));
	return cameraList_.back().get();
}

#ifdef NDEBUG
#else
#include "ImguiManager.h"
#include "ImGui.h"
void IFE::CameraManager::DebugGUI()
{
	static bool add = false;
	static bool fdelete = false;
	auto im = ImguiManager::Instance();
	im->NewGUI("CameraManager", (int32_t)ImGuiWindowFlags_MenuBar);
	if (im->NewMenuBar())
	{
		if (im->NewMenu("add"))
		{
			im->MenuItem("Add", &add);
			im->EndMenu();
		}
		if (im->NewMenu("delete"))
		{
			im->MenuItem("delete", &fdelete);
			im->EndMenu();
		}
		im->EndMenuBar();
	}

	im->TextGUI("CameraManager");

	if (add)
	{
		if (im->CollapsingHeaderGUI("New Camera"))
		{
			static std::string name;
			im->InputTextGUI("New Camera Name", name);
			if (im->ButtonGUI("Add Camera"))
			{
				if (name == "")
				{
					im->TextGUI("error : no name has been set");
				}
				else if (SearchName(name))
				{
					im->TextGUI("error : an camera with the same name exists");
				}
				else
				{
					AddInitialize(name);
					im->TextGUI("success");
				}
			}
		}
	}

	for (auto& itr : cameraList_)
	{
		itr->DebugGUI(fdelete);
	}

	im->EndGUI();

	for (auto& itr : cameraList_)
	{
		itr->ComponentGUI();
	}
}
void IFE::CameraManager::OutputScene()
{
	JsonManager* jm = JsonManager::Instance();
	nlohmann::json& j = jm->GetJsonData();
	uint32_t i = 0;
	for (auto& itr : cameraList_)
	{
		itr->OutputScene(j[i]);
		i++;
	}
	jm->Output("CameraManager");
}
#endif

void IFE::CameraManager::LoadingScene()
{
	sActivCamera_->GetView()->Initialze({ 0,10,-50 }, { 0,10,1 }, { 0,1,0 });

	//JsonManager* jm = JsonManager::Instance();
	//jm->Input("CameraManager");
	//nlohmann::json js = jm->GetJsonData();
	//for (auto& j : js)
	//{
	//	Camera* camera;
	//	Add(j["name"]);
	//	camera->LoadingScene(j);
	//	camera->Initialize();
	//}
}

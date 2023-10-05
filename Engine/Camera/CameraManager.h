#pragma once
#include "Camera.h"
#include "Warning.h"
WarningDisabled
#include <list>
#include <memory>
WarningDisabledPop


namespace IFE
{
	class CameraManager
	{
		CameraManager() {}
		CameraManager(const CameraManager&) {}
		CameraManager& operator=(const CameraManager&) {}
		~CameraManager();

		std::list<std::unique_ptr<Camera>>cameraList_;

	public:
		static Camera* sActivCamera_;
		inline static Camera* sNextCamera_ = nullptr;
		static CameraManager* Instance();
		void Initialize();
		void LoadingScene();
		void Update();
		void Draw();
		void Reset();
		Camera* Add(const std::string& cameraName);
		Camera* AddInitialize(const std::string& cameraName);
		bool SearchName(const std::string& name);
		Camera* GetCamera(const std::string& name);
		void SetActiveCamera(const std::string& name);

#ifdef NDEBUG
#else
		void DebugUpdate();
		void DebugGUI();
		void OutputScene();
#endif
	};
}

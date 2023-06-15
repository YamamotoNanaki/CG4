#pragma once
#include "Camera.h"

namespace IFE
{
	class CameraManager
	{
		CameraManager() {}
		CameraManager(const CameraManager&) {}
		CameraManager& operator=(const CameraManager&) {}
		~CameraManager();

	public:
		static Camera* sActivCamera_;
		static CameraManager* Instance();
		void Initialize();
		void Update();

#ifdef _DEBUG
		void DebugGUI();
#endif
	};
}

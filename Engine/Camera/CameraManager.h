#pragma once
#include "Camera.h"
#include "ConstBuffer.h"
#include "ConstStruct.h"
#include <list>

namespace IFE
{
	class CameraManager
	{
		CameraManager() {}
		CameraManager(const CameraManager&) {}
		CameraManager& operator=(const CameraManager&) {}
		~CameraManager();

		std::list<std::unique_ptr<Camera>>cameraList;
		ConstBuffer<ConstBufferCamera2D>camera2D_;
		Matrix mat2d_;
	public:
		static Camera* sActivCamera_;
		static CameraManager* Instance();
		void Initialize();
		void Update();
		void Draw();

#ifdef _DEBUG
		void DebugGUI();
#endif
	};
}

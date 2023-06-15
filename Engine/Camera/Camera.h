#pragma once
#include "View.h"
#include "Projection.h"
#include "Quaternion.h"
#include "IFMath.h"

#pragma warning(push)
#pragma warning(disable:4244)
#include <memory>
#pragma warning(pop)

namespace IFE
{
	class Camera
	{
		std::unique_ptr<View> matView_ = nullptr;
		std::unique_ptr<Projection> matPro_ = nullptr;
		Quaternion rotation_;
		Float3 position_;
		Float3 eulerAngleDegrees_;

	public:
		void CameraInitialize();
		void CameraUpdate();
		View* GetView();
		Projection* GetProjection();
		~Camera();
	};
}

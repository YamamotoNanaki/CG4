#pragma once
#include "View.h"
#include "Projection.h"
#include "Quaternion.h"
#include "IFMath.h"
#include "ComponentManager.h"

#pragma warning(push)
#pragma warning(disable:4244)
#include <memory>
#pragma warning(pop)

namespace IFE
{
	class Camera :public ComponentManager
	{
		View view_;
		Projection projection_;
		Quaternion rotation_;
		Float3 position_;
		Float3 eulerAngleDegrees_;

	public:
		Camera();
		void CameraInitialize();
		void CameraUpdate();
		View* GetView();
		Projection* GetProjection();
		~Camera();
	};
}

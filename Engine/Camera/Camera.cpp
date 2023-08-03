#include "Camera.h"
#include "Debug.h"
#include "WindowsAPI.h"

using namespace IFE;
using namespace std;

Camera::Camera()
{
	SetCameraPtr(this);
}

void IFE::Camera::CameraInitialize()
{
	view_ ;
	projection_;
}

void IFE::Camera::CameraUpdate()
{
	view_.Update();
	projection_.Update();
}

View* IFE::Camera::GetView()
{
	return &view_;
}

Projection* IFE::Camera::GetProjection()
{
	return &projection_;
}

IFE::Camera::~Camera()
{
}

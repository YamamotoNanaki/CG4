#include "Camera.h"
#include "Debug.h"
#include "WindowsAPI.h"

using namespace IFE;
using namespace std;

void IFE::Camera::CameraInitialize()
{
	matView_ = make_unique<View>() ;
	matPro_ = make_unique<Projection>(45, 1280, 720);
}

void IFE::Camera::CameraUpdate()
{
	matView_->Update();
	matPro_->Update();
}

View* IFE::Camera::GetView()
{
	return matView_.get();
}

Projection* IFE::Camera::GetProjection()
{
	return matPro_.get();
}

IFE::Camera::~Camera()
{
}

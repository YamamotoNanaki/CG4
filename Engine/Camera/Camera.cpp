#include "Camera.h"
#include "Debug.h"
#include "WindowsAPI.h"

using namespace IFE;
using namespace std;

void IFE::Camera::CameraInitialize()
{
	matView = make_unique<View>() ;
	matPro = make_unique<Projection>(45, 1280, 720);
}

void IFE::Camera::CameraUpdate()
{
	matView->Update();
	matPro->Update();
}

View* IFE::Camera::GetView()
{
	return matView.get();
}

Projection* IFE::Camera::GetProjection()
{
	return matPro.get();
}

IFE::Camera::~Camera()
{
}

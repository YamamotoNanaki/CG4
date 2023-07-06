#include "Camera.h"
#include "Debug.h"
#include "WindowsAPI.h"

using namespace IFE;
using namespace std;

void IFE::Camera::CameraInitialize()
{
	matView_ = make_unique<View>();
	matPro_ = make_unique<Projection>(45.f, 1280.f, 720.f);
	mapPtr = constBuffCamera_.GetCBMapObject();
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

void IFE::Camera::Draw()
{
	constBuffCamera_.SetConstBuffView(0);
}

IFE::Camera::~Camera()
{
}

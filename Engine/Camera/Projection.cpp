#include "Projection.h"

using namespace IFE;

Projection::Projection(float radian, float winWidth, float winHeight, float nearZ, float farZ)
	:fovAngle(radian), winWidth(winWidth), winHeight(winHeight), nearZ(nearZ), farZ(farZ)
{
	Inisialize(radian, winWidth, winHeight, nearZ, farZ);
}

void Projection::Inisialize(float radian, float ww, float wh, float nz, float fz)
{
	matProjection = MatrixPerspectiveFovLH(
		ConvertToRadians(radian), (float)ww / wh, nz, fz);
}

void IFE::Projection::Update()
{
	matProjection = MatrixPerspectiveFovLH(
		ConvertToRadians(fovAngle), (float)winWidth / winHeight, nearZ, farZ);
}

Matrix Projection::Get() const
{
	return matProjection;
}

Matrix* IFE::Projection::GetAddressOf()
{
	return &matProjection;
}

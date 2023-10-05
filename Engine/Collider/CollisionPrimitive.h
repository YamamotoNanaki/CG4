#pragma once
#include "IFMath.h"
#include "Warning.h"
WarningDisabled
#include <vector>
WarningDisabledPop


namespace IFE
{
	struct Sphere
	{
		Vector3 center{};
		float radius = 1.0f;
		Sphere(Vector3 center, float radius = 1.0f);
	};
	struct Plane
	{
		Vector3 normal = { 0,1,0 };
		float distance = 0.0f;
	};
	struct Triangle
	{
		Vector3 p0;
		Vector3 p1;
		Vector3 p2;
		Vector3 normal;
		void ComputeNormal();
	};
	struct Ray
	{
		Vector3 start{};
		Vector3 dir = { 1,0,0 };
	};
	class FBXModel;
	struct MeshCollider
	{
		std::vector<Triangle>triangles;
		Matrix* matWorld_;
		Matrix invMatWorld;
		MeshCollider(FBXModel* model, Matrix* matWorld);
		bool CheckCollisionSphere(const Sphere& sphere, Vector3* inter = nullptr, Vector3* reject = nullptr);
		bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, Vector3* inter = nullptr);
		~MeshCollider() {};
	};
	class Collider;
	struct RaycastHit
	{
		//衝突したコライダー
		Collider* collider = nullptr;
		// 衝突点
		Vector3 inter;
		// 衝突点までの距離
		float distance = 0.0f;
	};
}


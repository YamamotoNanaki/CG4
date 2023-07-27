#include "CollideManager.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "Object3D.h"
#include "Transform.h"

using namespace IFE;
using namespace std;

CollideManager* CollideManager::Instance()
{
	static CollideManager inst;
	return &inst;
}

void IFE::CollideManager::CollidersUpdate()
{
	if (colliders_.size() < 2)return;
	list<Collider*>::iterator itA;
	list<Collider*>::iterator itB;

	itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != colliders_.end(); ++itB)
		{
			Collider* colA = *itA;
			Collider* colB = *itB;

			//�Ƃ��ɋ�
			if (colA->GetColliderType() == ColliderType::SPHERE && colB->GetColliderType() == ColliderType::SPHERE)
			{
				Sphere SphereA(colA->GetColliderPosition(), colA->GetColliderScale().x);
				Sphere SphereB(colB->GetColliderPosition(), colB->GetColliderScale().x);
				Vector3 inter;
				Vector3 reject;
				if (Collision::CheckSphere(SphereA, SphereB, &inter, &reject))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					PushBack(colA, colB, reject);
					colA->objectPtr_->OnColliderHit(colB);
					colB->objectPtr_->OnColliderHit(colA);
				}
			}
			else if (colA->GetColliderType() == ColliderType::MESH && colB->GetColliderType() == ColliderType::SPHERE)
			{
				MeshCollider* mesh = colA->GetMeshCollider();
				Sphere sphere(colB->GetColliderPosition(), colB->GetColliderScale().x);
				Vector3 inter;
				if (mesh->CheckCollisionSphere(sphere, &inter))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					colA->objectPtr_->OnColliderHit(colB);
					colB->objectPtr_->OnColliderHit(colA);
				}
			}
			else if (colA->GetColliderType() == ColliderType::SPHERE && colB->GetColliderType() == ColliderType::MESH)
			{
				MeshCollider* mesh = colB->GetMeshCollider();
				Sphere sphere(colA->GetColliderPosition(), colA->GetColliderScale().x);
				Vector3 inter;
				if (mesh->CheckCollisionSphere(sphere, &inter))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					colA->objectPtr_->OnColliderHit(colB);
					colB->objectPtr_->OnColliderHit(colA);
				}
			}
		}
	}
}

void IFE::CollideManager::Reset()
{
	colliders_.clear();
}

void IFE::CollideManager::ColliderSet(Collider* collider)
{
	colliders_.push_back(collider);
}

bool IFE::CollideManager::Raycast(const Ray& ray, float maxDistance)
{
	return Raycast(ray, (uint16_t)Attribute::ALL, maxDistance);
}

bool IFE::CollideManager::Raycast(const Ray& ray, uint16_t attribute, float maxDistance)
{
	bool result = false;
	std::list<Collider*>::iterator it;
	std::list<Collider*>::iterator it_hit;
	float distance = maxDistance;
	Vector3 inter;

	// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
	it = colliders_.begin();
	for (; it != colliders_.end(); ++it) {
		Collider* colA = *it;

		// ����������Ȃ���΃X�L�b�v
		if (!(colA->attribute_ & attribute)) {
			continue;
		}

		if (colA->GetColliderType() == ColliderType::SPHERE) {
			Sphere* sphere = dynamic_cast<Sphere*>(colA);

			float tempDistance;
			Vector3 tempInter;

			if (!Collision::CheckRaySphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		else if (colA->GetColliderType() == ColliderType::MESH) {
			MeshCollider* meshCollider = colA->GetMeshCollider();

			float tempDistance;
			Vector3 tempInter;
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	//if (result && hitInfo) {
	//	hitInfo->distance = distance;
	//	hitInfo->inter = inter;
	//	hitInfo->collider = *it_hit;
	//	hitInfo->object = hitInfo->collider->GetObject3d();
	//}

	return result;
}

void IFE::CollideManager::PushBack(Collider* colA, Collider* colB, const Vector3& reject)
{
	// �n�ʔ��肵�����l
	static const float threshold = cosf(ConvertToRadians(30.0f));
	static const Vector3 up = { 0,1,0 };

	if (colA->GetPushBackFlag() && colB->GetPushBackFlag())
	{
		Vector3 rejectDir = Vector3Normalize(reject);
		float cos = Vector3Dot(rejectDir, up);
		if (-threshold < cos && cos < threshold)
		{
			colA->transform_->MovePushBack(reject / 2);
			colB->transform_->MovePushBack(-reject / 2);
		}
	}
	else if (colA->GetPushBackFlag())
	{
		Vector3 rejectDir = Vector3Normalize(reject);
		float cos = Vector3Dot(rejectDir, up);
		if (-threshold < cos && cos < threshold)
		{
			colA->transform_->MovePushBack(reject);
		}
	}
	else if (colB->GetPushBackFlag())
	{
		Vector3 rejectDir = Vector3Normalize(reject);
		float cos = Vector3Dot(rejectDir, up);
		if (-threshold < cos && cos < threshold)
		{
			colB->transform_->MovePushBack(-reject);
		}
	}
}

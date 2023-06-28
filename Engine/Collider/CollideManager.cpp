#include "CollideManager.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "Object3D.h"

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

			//‚Æ‚à‚É‹…
			if (colA->GetColliderType() == ColliderType::SPHERE && colB->GetColliderType() == ColliderType::SPHERE)
			{
				Sphere SphereA(colA->GetColliderPosition(), colA->GetColliderScale().x);
				Sphere SphereB(colB->GetColliderPosition(), colB->GetColliderScale().x);
				Vector3 inter;
				if (Collision::CheckSphere(SphereA, SphereB, &inter))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					colA->objectPtr_->OnColliderHit(colB);
					colB->objectPtr_->OnColliderHit(colA);
				}
			}
			//else if (colA->GetShapeType() == COLLISIONSHAPE_MESH && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			//{
			//    MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
			//    Sphere* sphere = dynamic_cast<Sphere*>(colB);
			//    Vector3 inter;
			//    if (meshCollider->CheckCollisionSphere(*sphere, &inter))
			//    {
			//        colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
			//        colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
			//    }
			//}
			//else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_MESH)
			//{
			//    MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
			//    Sphere* sphere = dynamic_cast<Sphere*>(colA);
			//    Vector3 inter;
			//    if (meshCollider->CheckCollisionSphere(*sphere, &inter))
			//    {
			//        colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
			//        colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
			//    }
			//}
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

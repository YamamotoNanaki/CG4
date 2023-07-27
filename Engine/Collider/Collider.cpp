#include "Collider.h"
#include "CollideManager.h"
#include "Transform.h"
#include "Object3D.h"
#include "FBXModel.h"

using namespace IFE;

void IFE::Collider::Initialize()
{
	if (colliderType_ == ColliderType::MESH)
	{
		auto mesh = new MeshCollider(dynamic_cast<FBXModel*>(objectPtr_->GetModel()), &transform_->matWorld_);
		meshCollider_ =std::unique_ptr<MeshCollider>(mesh);
	}
}

void IFE::Collider::Draw()
{
}

void IFE::Collider::Update()
{
	CollideManager::Instance()->ColliderSet(this);
}

Float3 IFE::Collider::GetOffsetPosition()
{
	return offsetPosition_;
}

Float3 IFE::Collider::GetOffsetScale()
{
	return offsetScale_;
}

Float3 IFE::Collider::GetColliderPosition()
{
	return transform_->position_ + offsetPosition_;
}

Float3 IFE::Collider::GetColliderScale()
{
	return transform_->scale_ + offsetScale_;
}

ColliderType IFE::Collider::GetColliderType()
{
	return colliderType_;
}

void IFE::Collider::SetColliderType(const ColliderType& colliderType)
{
	colliderType_ = colliderType;
}

void IFE::Collider::SetOffsetPosition(const Float3& pos)
{
	offsetPosition_ = pos;
}

void IFE::Collider::SetOffsetScale(const Float3& sca)
{
	offsetScale_ = sca;
}

MeshCollider* IFE::Collider::GetMeshCollider()
{
	return meshCollider_.get();
}

bool IFE::Collider::GetPushBackFlag()
{
	return pushBack_;
}

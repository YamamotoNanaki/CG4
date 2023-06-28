#pragma once
#include "Component.h"

namespace IFE
{
	enum class ColliderType
	{
		SPHERE,
	};
	class Collider : public Component
	{
	private:
		Float3 offsetPosition_;
		Float3 offsetScale_;
		ColliderType colliderType_;
	public:
		Vector3 interPoint_;

	public:
		void Draw()override;
		void Update()override;

		Float3 GetOffsetPosition();
		Float3 GetOffsetScale();
		Float3 GetColliderPosition();
		Float3 GetColliderScale();
		ColliderType GetColliderType();
		void SetColliderType(const ColliderType& colliderType);
		void SetOffsetPosition(const Float3& pos);
		void SetOffsetScale(const Float3& sca);
	};
}

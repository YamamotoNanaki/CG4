#pragma once
#include "Component.h"

namespace IFE
{
	enum class ColliderType
	{
		SPHERE, MESH
	};
	enum class Attribute
	{
		LANDSHAPE = 0b1 << 0, ALLIES = 0b1 << 1, ENEMYS = 0b1 << 2,
		ALL = 0xffff
	};
	struct MeshCollider;
	class Collider : public Component
	{
	private:
		Float3 offsetPosition_;
		Float3 offsetScale_;
		ColliderType colliderType_;
		bool pushBack_ = false;
		bool groundJudge_ = false;

		std::unique_ptr<MeshCollider> meshCollider_;
	public:
		Vector3 interPoint_;
		uint16_t attribute_ = (uint16_t)Attribute::ALL;
		bool onGround = false;
		RaycastHit groundHit;

	public:
		void Initialize()override;
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

		MeshCollider* GetMeshCollider();
		bool GetPushBackFlag();
		bool GetGroundJudgeFlag();

		void LoadingComponent(nlohmann::json& json)override;

#ifdef _DEBUG
		void OutputComponent(nlohmann::json& json)override;
		void ComponentDebugGUI()override;
#endif
	};
}

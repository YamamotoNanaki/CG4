#pragma once
#include "Component.h"
#include "ConstBuffer.h"
#include "ConstStruct.h"
#include "Camera.h"
#include "Quaternion.h"
#include <memory>

namespace IFE
{
	enum class BillboardFlag
	{
		Default, Billboard, YBillboard
	};

	class Transform : public Component
	{
		using Component::Component;
		Camera* camera_ = nullptr;
		std::unique_ptr<ConstBuffer<ConstBufferDataTransform>> transformBuffer_;
		ConstBufferDataTransform* constMapTransform_ = nullptr;

		Float3 lossyScale_;
	public:
		bool eulerFlag_ = false;
		Float3 eulerAngleDegrees_;
		uint8_t billbord_ = 0;
		Matrix matWorld_;
		Matrix matScale_;
		Matrix matRot_;
		Matrix matTrans_;
		Float3 scale_ = { 1,1,1 };
		Float3 position_ = { 0,0,0 };
		Quaternion rotation_;

		Transform* parent_ = nullptr;

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void UpdateMatrix();
		void Copy(Component* component);
		Vector3 TransformPoint(const Vector3& position);
		Vector3 InverseTransformPoint(const Vector3& position);
		Float3 GetLossyScale();
		Vector3 GetForwardVector();
		Vector3 GetUpVector();
		Vector3 GetRightVector();
		void MovePushBack(Vector3 move);

		/// <summary>
		///ワールド座標を取得する
		/// </summary>
		Vector3 GetWorldPosition();

		/// <summary>
		///ワールド座標を代入する
		/// </summary>
		void SetWorldPosition(const Vector3& worldPos);

		~Transform();
#ifdef _DEBUG
		void DebugGUI();
		void OutputComponent(nlohmann::json&json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	};

	class Transform2D : public Component
	{
		using Component::Component;
		Camera* camera_ = nullptr;
		std::unique_ptr<ConstBuffer<ConstBufferMatrix>> transformBuffer_;
		ConstBufferMatrix* constMapTransform_ = nullptr;
		static Matrix matPro_;
	public:
		float rotation_;
		Matrix matWorld_;
		Float2 scale_ = { 1,1 };
		Float2 position_ = { 0,0 };

	public:
		static void StaticInitialize();
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void UpdateMatrix();

		~Transform2D();
#ifdef _DEBUG
		void DebugGUI();
		void OutputComponent(nlohmann::json&json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	};

	class TransformParticle : public Component
	{
		using Component::Component;
		Camera* camera_ = nullptr;
		std::unique_ptr<ConstBuffer<ConstBufferBillboard>> transformBuffer_;
		ConstBufferBillboard* constMapTransform_ = nullptr;

		Float3 lossyScale_;
	public:
		bool eulerFlag_ = false;
		Float3 eulerAngleDegrees_;
		uint8_t billbord_ = 0;
		Matrix matWorld_;
		Matrix matScale_;
		Matrix matRot_;
		Matrix matTrans_;
		Float3 scale_ = { 1,1,1 };
		Float3 position_ = { 0,0,0 };
		Quaternion rotation_;

		TransformParticle* parent_ = nullptr;

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void UpdateMatrix();
		Vector3 TransformPoint(const Vector3& position);
		Vector3 InverseTransformPoint(const Vector3& position);
		Float3 GetLossyScale();

		/// <summary>
		///ワールド座標を取得する
		/// </summary>
		Vector3 GetWorldPosition();

		/// <summary>
		///ワールド座標を代入する
		/// </summary>
		void SetWorldPosition(const Vector3& worldPos);

		~TransformParticle();
#ifdef _DEBUG
		void DebugGUI();
		void OutputComponent(nlohmann::json& json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	};
}

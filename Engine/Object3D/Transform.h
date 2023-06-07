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
		Camera* camera = nullptr;
		std::unique_ptr<ConstBuffer<ConstBufferDataTransform>> transformBuffer;
		ConstBufferDataTransform* constMapTransform = nullptr;

		Float3 lossyScale;
	public:
		bool eulerFlag = false;
		Float3 eulerAngleDegrees;
		uint8_t billbord = 0;
		Matrix matWorld;
		Matrix matScale_;
		Matrix matRot_;
		Matrix matTrans_;
		Float3 scale = { 1,1,1 };
		Float3 position = { 0,0,0 };
		Quaternion rotation;

		Transform* parent = nullptr;

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void UpdateMatrix();
		void Copy(Component* component);
		Vector3 TransformPoint(Vector3 position);
		Vector3 InverseTransformPoint(Vector3 position);
		Float3 GetLossyScale();
		Vector3 GetForwardVector();
		Vector3 GetUpVector();
		Vector3 GetRightVector();

		/// <summary>
		///ワールド座標を取得する
		/// </summary>
		Vector3 GetWorldPosition();

		/// <summary>
		///ワールド座標を代入する
		/// </summary>
		void SetWorldPosition(Vector3 worldPos);

		~Transform();
#ifdef _DEBUG
		void DebugGUI();
		void OutputComponent()override;
#endif
		void LoadingComponent()override;
	};

	class Transform2D : public Component
	{
		using Component::Component;
		Camera* camera = nullptr;
		std::unique_ptr<ConstBuffer<ConstBufferMatrix>> transformBuffer;
		ConstBufferMatrix* constMapTransform = nullptr;
		static Matrix matPro;
	public:
		float rotation;
		Matrix matWorld;
		Float2 scale = { 1,1 };
		Float2 position = { 0,0 };

	public:
		static void StaticInitialize();
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void UpdateMatrix();

		~Transform2D();
#ifdef _DEBUG
		void DebugGUI();
		void OutputComponent()override;
#endif
		void LoadingComponent()override;
	};

	class TransformParticle : public Component
	{
		using Component::Component;
		Camera* camera = nullptr;
		std::unique_ptr<ConstBuffer<ConstBufferBillboard>> transformBuffer;
		ConstBufferBillboard* constMapTransform = nullptr;

		Float3 lossyScale;
	public:
		bool eulerFlag = false;
		uint8_t billbord = 0;
		Matrix matWorld;
		Matrix matScale_;
		Matrix matRot_;
		Matrix matTrans_;
		Float3 scale = { 1,1,1 };
		Float3 position = { 0,0,0 };
		float rotation;

		TransformParticle* parent = nullptr;

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void UpdateMatrix();
		Vector3 TransformPoint(Vector3 position);
		Vector3 InverseTransformPoint(Vector3 position);
		Float3 GetLossyScale();

		/// <summary>
		///ワールド座標を取得する
		/// </summary>
		Vector3 GetWorldPosition();

		/// <summary>
		///ワールド座標を代入する
		/// </summary>
		void SetWorldPosition(Vector3 worldPos);

		~TransformParticle();
	};
}

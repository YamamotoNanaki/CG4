#pragma once
#include "Texture.h"
#include "IFMath.h"
#include "ConstBuffer.h"
#include "ConstStruct.h"
#include "Component.h"
#include <string>
#include <memory>

namespace IFE
{
	class Material : public Component
	{
		using Component::Component;
	private:
		std::string name_ = "";
		Float3 ambient_ = { 0.3f,0.3f,0.3f };
		Float3 diffuse_ = { 0.3f,0.3f,0.3f };
		Float3 specular_ = { 0.3f,0.3f,0.3f };
	public:
		Float4 color_ = { 1,1,1,1 };
		float alpha_ = 1.0f;
		Texture* tex_ = nullptr;

		std::unique_ptr<ConstBuffer<ConstBufferMaterial>> materialBuffer_;
		ConstBufferMaterial* constMapMaterial_ = nullptr;

	public:
		void Initialize();
		void SetDefaultParameter();
		void Update();
		void Draw();
		void SetTexture(Texture* texture);
		Float4 GetColor();
		void SetColor(const Float4& color);
		Float4 GetColor255();
		void SetColor255(const Float4& color);
		void Copy(Component* component);
#ifdef _DEBUG
		void DebugGUI();
		void OutputComponent();
#endif
		void LoadingComponent();
	};
}

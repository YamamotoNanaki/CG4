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
		std::string name = "";
		Float3 ambient = { 0.3f,0.3f,0.3f };
		Float3 diffuse = { 0.3f,0.3f,0.3f };
		Float3 specular = { 0.3f,0.3f,0.3f };
	public:
		Float4 color = { 1,1,1,1 };
		float alpha = 1.0f;
		Texture* tex = nullptr;

		std::unique_ptr<ConstBuffer<ConstBufferMaterial>> materialBuffer;
		ConstBufferMaterial* constMapMaterial = nullptr;

	public:
		void Initialize();
		void SetDefaultParameter();
		void Update();
		void Draw();
		void SetTexture(Texture* texture);
		Float4 GetColor();
		void SetColor(Float4 color);
		Float4 GetColor255();
		void SetColor255(Float4 color);
		void Copy(Component* component);
#ifdef _DEBUG
		void DebugGUI();
		void OutputComponent();
#endif
		void LoadingComponent();
	};
}

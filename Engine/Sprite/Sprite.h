#pragma once
#include "IFMath.h"
#include "ComponentManager.h"
#include "Component.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "GraphicsPipeline.h"
#include "ConstStruct.h"
#include <string>

namespace IFE
{
	class Transform2D;
	class Sprite :public ComponentManager2D
	{
	protected:
		static GraphicsPipeline gp;

	protected:
		VertexBuffer<Vertex2D> vb;
	public:
		//アフィン変換情報
		//Float2 scale = { 1,1 };
		//float rotation = 0;
		//Float2 position = { 0,0 };
		Float2 size = { 100,100 };
		//ワールド変換行列
		//Matrix matWorld;
		Float2 texBase = { 0,0 };
		Float2 texSize = { 100,100 };
		Float2 anchorpoint = { 0.5,0.5 };
		bool flipX = false;
		bool flipY = false;
		std::string spriteName;
		Texture* tex = nullptr;
		bool drawFlag = true;
		bool deleteFlag = false;
		bool isActive = true;
		Transform2D* transform = nullptr;
	public:
		Sprite();
		static void StaticInitialize();
		void BufferInitialize();
		void SPRITEInitialize(std::string texName = "white.png", Float2 size = { 100,100 }, Float2 anchorpoint = { 0.5,0.5 }, bool flipX = false, bool flipY = false);
		void Initialize(std::string texName = "white.png", Float2 size = { 100,100 }, Float2 anchorpoint = { 0.5,0.5 }, bool flipX = false, bool flipY = false);
		void TransferVertex();
		static void DrawBefore();
		void Update();
		void Draw();
		void SetSize(Float2 size);
		void SetTextureRect(Float2 texBase, Float2 texSize);
		void SetComponent(Component* component);
		void SetComponentFront(Component* component);
		void SetTexture(std::string texName);

#ifdef _DEBUG
		virtual void DebugGUI(bool fdelete, bool fmove, std::string* str);
		void ComponentGUI();
		void DebugUpdate();
		void OutputScene();
#endif
	};
}
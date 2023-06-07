#include "Sprite.h"
//#include "SpriteManager.h"
#include <cassert>
#include "TextureManager.h"
#include "Debug.h"
#include "imgui.h"
#include "GraphicsAPI.h"
#include "Transform.h"
#include "ColorBuffer.h"

using namespace IFE;
using namespace Microsoft::WRL;

GraphicsPipeline Sprite::gp;

IFE::Sprite::Sprite()
{
	SetSpritePtr(this);
}

void IFE::Sprite::StaticInitialize()
{
	gp.CreateBasic2DGraphicsPipeLine();
}

void IFE::Sprite::BufferInitialize()
{
	Vertex2D vertices[4];

	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;

	if (flipX)
	{
		left = -left;
		right = -right;
	}

	if (flipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };
	float tex_left = 0;
	float tex_right = 1;
	float tex_top = 0;
	float tex_bottom = 1;
	vertices[LB].uv = { tex_left,	tex_bottom };
	vertices[LT].uv = { tex_left,	tex_top };
	vertices[RB].uv = { tex_right,	tex_bottom };
	vertices[RT].uv = { tex_right,	tex_top };

	vb.SetVerticle(vertices, _countof(vertices));
	vb.Initialize();
}

void IFE::Sprite::SPRITEInitialize(std::string texName, Float2 s, Float2 a, bool x, bool y)
{
	this->tex = TextureManager::Instance()->GetTexture(texName);

	this->anchorpoint = a;
	this->size = s;
	this->flipX = x;
	this->flipY = y;

	BufferInitialize();
	AddComponent<ColorBuffer>();
	AddComponent<Transform2D>();
	ComponentManager2D::Initialize();
	transform = GetComponent<Transform2D>();
}

void IFE::Sprite::Initialize(std::string texName, Float2 s, Float2 a, bool x, bool y)
{
	if (tex == nullptr)this->tex = TextureManager::Instance()->GetTexture(texName);

	this->anchorpoint = a;
	this->size = s;
	this->flipX = x;
	this->flipY = y;

	BufferInitialize();
	ComponentManager2D::Initialize();
	transform = GetComponent<Transform2D>();
}

void IFE::Sprite::DrawBefore()
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	gp.SetDrawBlendMode();
}

void IFE::Sprite::Update()
{
	if (!isActive)return;
	ComponentManager2D::Update();
}

void IFE::Sprite::Draw()
{
	if (!drawFlag)return;
	if (!isActive)return;

	ComponentManager2D::Draw();
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, vb.GetVBView());
	//描画コマンド
	tex->SetTexture(2);
	commandList->DrawInstanced((UINT)vb.GetSize(), 1, 0, 0);
}

void Sprite::SetSize(Float2 s)
{
	this->size = s;
	TransferVertex();
}

void Sprite::SetTextureRect(Float2 b, Float2 s)
{
	this->texBase = b;
	this->texSize = s;

	// 頂点バッファへのデータ転送
	TransferVertex();
}

void IFE::Sprite::SetComponent(Component* component)
{
	AddComponentBack<Component>(component);
}

void IFE::Sprite::SetComponentFront(Component* component)
{
	AddComponent<Component>(component);
}

void IFE::Sprite::SetTexture(std::string texName)
{
	this->tex = TextureManager::Instance()->GetTexture(texName);
}

#ifdef _DEBUG
#include "ImguiManager.h"

void IFE::Sprite::DebugGUI(bool fdelete, bool fmove, std::string* str)
{
	bool m = false;
	if (ImguiManager::Instance()->SpriteGUI(spriteName, fdelete, fmove, &m))
	{
		deleteFlag = true;
	}
	if (m)
	{
		*str = spriteName;
	}
}

void IFE::Sprite::ComponentGUI()
{
	std::function<void(Component*)> addFunc = [&](Component* com)
	{
		SetComponentFront(com);
	};
	std::function<void(void)>f = [&]()
	{
		ComponentManager2D::DebugGUI();
	};
	//std::function<void(std::string)>texFunc = [&](std::string name)
	//{
	//	tex = TextureManager::Instance()->GetTexture(name);
	//};
	ImguiManager::Instance()->ComponentGUI2D(spriteName, f, addFunc/*, texFunc*/);
}

void IFE::Sprite::DebugUpdate()
{
	ComponentManager2D::DebugUpdate();
}
void IFE::Sprite::OutputScene()
{
	ComponentManager2D::OutputScene(spriteName);
}
#endif

void Sprite::TransferVertex()
{
	enum { LB, LT, RB, RT };
	Vertex2D vertices[4];

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (flipX)
	{
		left = -left;
		right = -right;
	}

	if (flipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };
	float tex_left = 0;
	float tex_right = 1;
	float tex_top = 1;
	float tex_bottom = 0;
	vertices[LB].uv = { tex_left,	tex_bottom };
	vertices[LT].uv = { tex_left,	tex_top };
	vertices[RB].uv = { tex_right,	tex_bottom };
	vertices[RT].uv = { tex_right,	tex_top };

	ComPtr<ID3D12Resource> texBuff = tex->texbuff;

	if (texBuff)
	{
		D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

		tex_left = texBase.x / resDesc.Width;
		tex_right = (texBase.x + texSize.x) / resDesc.Width;
		tex_top = texBase.y / resDesc.Height;
		tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,	tex_top }; // 左上
		vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,	tex_top }; // 右上
	}

	vb.Transfer(vertices, _countof(vertices));
}

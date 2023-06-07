#include "Material.h"
#include "ImguiManager.h"
#include "TextureManager.h"
#include "JsonManager.h"

using namespace std;
using namespace IFE;

void IFE::Material::Initialize()
{
	materialBuffer = make_unique<ConstBuffer<ConstBufferMaterial>>();
	constMapMaterial = materialBuffer->GetCBMapObject();
}

void IFE::Material::SetDefaultParameter()
{
	ambient = { 0.3f,0.3f,0.3f };
	diffuse = { 0.3f,0.3f,0.3f };
	specular = { 0.3f,0.3f,0.3f };
	alpha = 1.0f;
	name = "DefaultMaterial";
	constMapMaterial->alpha = alpha;
	constMapMaterial->color = color;
	constMapMaterial->ambient = ambient;
	constMapMaterial->diffuse = diffuse;
	constMapMaterial->specular = specular;
	tex = TextureManager::Instance()->GetTexture("white.png");
}

void IFE::Material::Update()
{
	constMapMaterial->alpha = alpha;
	constMapMaterial->color = color;
	constMapMaterial->ambient = ambient;
	constMapMaterial->diffuse = diffuse;
	constMapMaterial->specular = specular;
}

void IFE::Material::Draw()
{
	materialBuffer->SetConstBuffView(2);
	if (tex == nullptr)
	{
		tex = TextureManager::Instance()->GetTexture("white.png");
	}
	tex->SetTexture(4);
}

void IFE::Material::SetTexture(Texture* texture)
{
	tex = texture;
}

Float4 IFE::Material::GetColor()
{
	return color;
}

void IFE::Material::SetColor(Float4 c)
{
	color = c;
}

Float4 IFE::Material::GetColor255()
{
	Float4 c = { color.x * 255.f,color.y * 255.f,color.z * 255.f,color.w * 255.f };
	return c;
}

void IFE::Material::SetColor255(Float4 c)
{
	color = { c.x / 255.f,c.y / 255.f,c.z / 255.f,c.w / 255.f };
}

void IFE::Material::Copy(Component* component)
{
	Material* m = dynamic_cast<Material*>(component);
	if (m == nullptr)return;
	ambient = m->ambient;
	diffuse = m->diffuse;
	specular = m->specular;
	color = m->color;
	tex = m->tex;
}

#ifdef _DEBUG
void IFE::Material::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(std::string)> guiFunc2 = [&](std::string textureName)
	{
		tex = TextureManager::Instance()->GetTexture(textureName);
	};
	std::function<void(void)> guiFunc = [&]()
	{
		im->ColorEdit4GUI(&color, "color");
		im->DragFloat3GUI(&ambient, "ambient");
		im->DragFloat3GUI(&diffuse, "diffuse");
		im->DragFloat3GUI(&specular, "specular");
		im->ChangeTextureGUI(guiFunc2);
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName);
}

void IFE::Material::OutputComponent()
{
	JsonManager* j = JsonManager::Instance();
	j->OutputFloat4("color", color);
	j->OutputFloat3("ambient", ambient);
	j->OutputFloat3("diffuse", diffuse);
	j->OutputFloat3("specular", specular);
	j->OutputString("terxtureName", tex->texName);
}
#endif

void IFE::Material::LoadingComponent()
{
	JsonManager* j = JsonManager::Instance();
	color = j->InputFloat4("color");
	ambient = j->InputFloat3("ambient");
	diffuse = j->InputFloat3("diffuse");
	specular = j->InputFloat3("specular");
	SetTexture(TextureManager::Instance()->GetTexture(j->InputString("terxtureName")));
	materialBuffer = make_unique<ConstBuffer<ConstBufferMaterial>>();
	constMapMaterial = materialBuffer->GetCBMapObject();
}
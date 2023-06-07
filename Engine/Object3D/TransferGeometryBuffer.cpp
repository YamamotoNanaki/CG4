#include "TransferGeometryBuffer.h"
#include "ImguiManager.h"
#include "JsonManager.h"
#include "Material.h"

using namespace IFE;
using namespace std;

void IFE::TransferGeometryBuffer::Initialize()
{
	geometryBuffer = make_unique<ConstBuffer<ConstBufferModelGeometry>>();
	constMapGeometry = geometryBuffer->GetCBMapObject();
}

void IFE::TransferGeometryBuffer::Update()
{
	constMapGeometry->explosion = explosion;
	constMapGeometry->gravity = gravity;
	constMapGeometry->rotation = rotation;
	constMapGeometry->polygonSize = polygonSize;
	constMapGeometry->lightFlag = lightFlag;
	constMapGeometry->toonFlag = toonFlag;
}

void IFE::TransferGeometryBuffer::Draw()
{
	geometryBuffer->SetConstBuffView(1);
}

void IFE::TransferGeometryBuffer::Copy(Component* component)
{
	TransferGeometryBuffer* t = dynamic_cast<TransferGeometryBuffer*>(component);
	if (t == nullptr)return;
	 explosion = t->explosion;
	 gravity = t->gravity;
	 rotation = t->rotation;
	 polygonSize = t->polygonSize;
	lightFlag = t->lightFlag;
	toonFlag = t->toonFlag;
}

#ifdef _DEBUG

void IFE::TransferGeometryBuffer::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloatGUI(&explosion, "explosion");
		im->DragFloatGUI(&gravity, "gravity");
		im->DragFloatGUI(&rotation, "polygon rotation");
		im->DragFloatGUI(&polygonSize, "polygon size");
		im->CheckBoxGUI(&lightFlag, "lightFlag");
		im->CheckBoxGUI(&toonFlag, "toonFlag");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName);
}

void IFE::TransferGeometryBuffer::OutputComponent()
{
	JsonManager* j = JsonManager::Instance();
	j->OutputFloat("explosion", explosion);
	j->OutputFloat("gravity", gravity);
	j->OutputFloat("rotation", rotation);
	j->OutputFloat("polygonSize", polygonSize);
	j->OutputBool("lightFlag", lightFlag);
	j->OutputBool("toonFlag", toonFlag);
}

#endif

void IFE::TransferGeometryBuffer::LoadingComponent()
{
	JsonManager* j = JsonManager::Instance();
	explosion = j->InputFloat("explosion");
	gravity = j->InputFloat("gravity");
	rotation = j->InputFloat("rotation");
	polygonSize = j->InputFloat("polygonSize");
	lightFlag = j->InputBool("lightFlag");
	toonFlag = j->InputBool("toonFlag");
	geometryBuffer = make_unique<ConstBuffer<ConstBufferModelGeometry>>();
	constMapGeometry = geometryBuffer->GetCBMapObject();
}

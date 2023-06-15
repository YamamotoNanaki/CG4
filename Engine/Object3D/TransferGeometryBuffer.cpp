#include "TransferGeometryBuffer.h"
#include "ImguiManager.h"
#include "JsonManager.h"
#include "Material.h"

using namespace IFE;
using namespace std;

void IFE::TransferGeometryBuffer::Initialize()
{
	geometryBuffer_ = make_unique<ConstBuffer<ConstBufferModelGeometry>>();
	constMapGeometry_ = geometryBuffer_->GetCBMapObject();
}

void IFE::TransferGeometryBuffer::Update()
{
	constMapGeometry_->explosion = explosion_;
	constMapGeometry_->gravity = gravity_;
	constMapGeometry_->rotation = rotation_;
	constMapGeometry_->polygonSize = polygonSize_;
	constMapGeometry_->lightFlag = lightFlag_;
	constMapGeometry_->toonFlag = toonFlag_;
}

void IFE::TransferGeometryBuffer::Draw()
{
	geometryBuffer_->SetConstBuffView(1);
}

void IFE::TransferGeometryBuffer::Copy(Component* component)
{
	TransferGeometryBuffer* t = dynamic_cast<TransferGeometryBuffer*>(component);
	if (t == nullptr)return;
	 explosion_ = t->explosion_;
	 gravity_ = t->gravity_;
	 rotation_ = t->rotation_;
	 polygonSize_ = t->polygonSize_;
	lightFlag_ = t->lightFlag_;
	toonFlag_ = t->toonFlag_;
}

#ifdef _DEBUG

void IFE::TransferGeometryBuffer::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloatGUI(&explosion_, "explosion");
		im->DragFloatGUI(&gravity_, "gravity");
		im->DragFloatGUI(&rotation_, "polygon rotation");
		im->DragFloatGUI(&polygonSize_, "polygon size");
		im->CheckBoxGUI(&lightFlag_, "lightFlag");
		im->CheckBoxGUI(&toonFlag_, "toonFlag");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
}

void IFE::TransferGeometryBuffer::OutputComponent()
{
	JsonManager* j = JsonManager::Instance();
	j->OutputFloat("explosion", explosion_);
	j->OutputFloat("gravity", gravity_);
	j->OutputFloat("rotation", rotation_);
	j->OutputFloat("polygonSize", polygonSize_);
	j->OutputBool("lightFlag", lightFlag_);
	j->OutputBool("toonFlag", toonFlag_);
}

#endif

void IFE::TransferGeometryBuffer::LoadingComponent()
{
	JsonManager* j = JsonManager::Instance();
	explosion_ = j->InputFloat("explosion");
	gravity_ = j->InputFloat("gravity");
	rotation_ = j->InputFloat("rotation");
	polygonSize_ = j->InputFloat("polygonSize");
	lightFlag_ = j->InputBool("lightFlag");
	toonFlag_ = j->InputBool("toonFlag");
	geometryBuffer_ = make_unique<ConstBuffer<ConstBufferModelGeometry>>();
	constMapGeometry_ = geometryBuffer_->GetCBMapObject();
}

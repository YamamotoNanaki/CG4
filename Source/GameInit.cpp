#include "GameInit.h"
#include "ModelManager.h"
#include "FBXModel.h"
#include "ModelStruct.h"
#include "Mesh.h"
#include "Material.h"
#include "ImguiManager.h"

void GameInit::Initialize()
{
	SetInitMaterial("Bush1");
	SetInitMaterial("Rock1");
	SetInitMaterial("girl");
}

void GameInit::SetInitMaterial(std::string modelName)
{
	auto model = dynamic_cast<IFE::FBXModel*>(IFE::ModelManager::Instance()->GetModel(modelName));
	if (!model)return;
	for (auto& node : model->nodes_)
	{
		for (auto& mesh : node->meshes)
		{
			auto mat = mesh->GetMaterial();
			mat.color = col;
			mesh->SetMaterial(mat);
		}
	}
}

#ifdef NDEBUG
#else
void GameInit::ComponentDebugGUI()
{
	IFE::ImguiManager::Instance()->ColorEdit4GUI(&col, "color");
}

void GameInit::OutputComponent(nlohmann::json& json)
{
	IFE::JsonManager::Instance()->OutputFloat4(json, col);
}
#endif

void GameInit::LoadingComponent(nlohmann::json& json)
{
	col = IFE::JsonManager::Instance()->InputFloat4(json);
}

#include "ColorBuffer.h"
#include "ImguiManager.h"
#include "JsonManager.h"

using namespace IFE;
using namespace std;

void IFE::ColorBuffer::Initialize()
{
	colorBuffer_ = make_unique<ConstBuffer<ConstBufferColor>>();
	constMapColor_ = colorBuffer_->GetCBMapObject();
}

void IFE::ColorBuffer::Update()
{
	constMapColor_->color = color_;
}

void IFE::ColorBuffer::Draw()
{
	colorBuffer_->SetConstBuffView(0);
}

#ifdef _DEBUG
void IFE::ColorBuffer::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->ColorEdit4GUI(&color_, "color");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
}

void IFE::ColorBuffer::OutputComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	j->OutputFloat4(json["color"], color_);
}
#endif

void IFE::ColorBuffer::LoadingComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	color_ = j->InputFloat4(json["color"]);
	colorBuffer_ = make_unique<ConstBuffer<ConstBufferColor>>();
	constMapColor_ = colorBuffer_->GetCBMapObject();
}

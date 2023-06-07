#include "ColorBuffer.h"
#include "ImguiManager.h"
#include "JsonManager.h"

using namespace IFE;
using namespace std;

void IFE::ColorBuffer::Initialize()
{
	colorBuffer = make_unique<ConstBuffer<ConstBufferColor>>();
	constMapColor = colorBuffer->GetCBMapObject();
}

void IFE::ColorBuffer::Update()
{
	constMapColor->color = color;
}

void IFE::ColorBuffer::Draw()
{
	colorBuffer->SetConstBuffView(0);
}

#ifdef _DEBUG
void IFE::ColorBuffer::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->ColorEdit4GUI(&color, "color");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName);
}

void IFE::ColorBuffer::OutputComponent()
{
	JsonManager* j = JsonManager::Instance();
	j->OutputFloat4("color", color);
}
#endif

void IFE::ColorBuffer::LoadingComponent()
{
	JsonManager* j = JsonManager::Instance();
	color = j->InputFloat4("color");
	colorBuffer = make_unique<ConstBuffer<ConstBufferColor>>();
	constMapColor = colorBuffer->GetCBMapObject();
}

#pragma once
#include "Component.h"
#include "JsonManager.h"
#include <string>

class GameInit : public IFE::Component
{
private:
	inline static IFE::Float4 col = { 0.25,0.25,0.25,1 };

public:
	void Initialize()override;

private:
	void SetInitMaterial(std::string modelName);

#ifdef NDEBUG
#else
	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;
};


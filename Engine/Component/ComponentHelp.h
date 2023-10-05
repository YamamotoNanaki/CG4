#pragma once
#include "Warning.h"
WarningDisabled
#include <string>
WarningDisabledPop


namespace IFE
{
	class Component;
	Component* StringToComponent(const std::string& componentName);
}

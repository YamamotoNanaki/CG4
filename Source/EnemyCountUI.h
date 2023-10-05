#pragma once
#include "Component.h"
#include "TextureManager.h"
#include "Warning.h"
WarningDisabled
#include <array>
WarningDisabledPop


namespace IFE
{
    class EnemyCountUI : public Component
    {
        std::array<Texture*, 10>numberTextures_;
    public:
        void Initialize()override;
        void Update()override;
    };
}

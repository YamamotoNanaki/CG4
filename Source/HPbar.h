#pragma once
#include "Component.h"
#include "Player.h"

namespace IFE
{
    class HPbar : public Component
    {
        Player* playerPtr_;
        Float2 scale_;

    public:
        void Initialize();
        void Update();
    };
}

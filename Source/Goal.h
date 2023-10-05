#pragma once
#include "Component.h"
#include "Collider.h"


namespace IFE
{
    class Goal : public Component
    {
        bool goal_ = false;
    public:
        void Update()override;
        void OnColliderHit(Collider* collider)override;
    };
}

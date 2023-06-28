#pragma once
#include "Collider.h"
#include <list>

namespace IFE
{
	class CollideManager
	{
		std::list<Collider*>colliders_;
	private:
		CollideManager() {};
		CollideManager(const CollideManager& obj) = delete;
		CollideManager& operator=(const CollideManager& obj) = delete;
	public:
		static CollideManager* Instance();
		void CollidersUpdate();
		void Reset();
		void ColliderSet(Collider* collider);
	};
}

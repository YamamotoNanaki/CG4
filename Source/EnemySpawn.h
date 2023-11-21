#pragma once
#include "Component.h"
#include "Enemy.h"

class EnemySpawn : public IFE::Component
{
	std::list<IFE::Enemy*>enemys_;
	bool once_ = false;
public:
	void Initialize()override;
	void Update()override;
	void OnColliderHit(IFE::Collider* collider)override;
};


#pragma once
#include "Component.h"
#include "Enemy.h"
#include "Sprite.h"

struct EnemyData
{
	IFE::Enemy* ptr;
	std::string name;
	uint8_t hp = 1;
	float startTime;
	float endTime;
	bool flag = false;
	bool end = false;
};

class EnemySpawn : public IFE::Component
{
	std::list<EnemyData>enemys_;
	float timer_ = 0;
	float endTime_ = 30;
	bool once_ = false;
	bool end_ = false;

	IFE::Sprite* clook_;
	IFE::Sprite* needle_;
	IFE::Sprite* timeSprites_[2];
	std::array<IFE::Texture*, 10>numberTextures_;
public:
	void Initialize()override;
	void Update()override;
	void OnColliderHit(IFE::Collider* collider)override;

#ifdef NDEBUG
#else
	void OutputComponent(nlohmann::json& json)override;
	void ComponentDebugGUI()override;
#endif
	void LoadingComponent(nlohmann::json & json)override;
};


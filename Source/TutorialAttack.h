#pragma once
#include "Component.h"

class TutorialAttack : public IFE::Component
{
private:
	IFE::Sprite* RButton_ = nullptr;
	IFE::Sprite* attack_ = nullptr;
	bool stayPlayer_ = false;
	bool displayEnd_ = false;
	bool tutorialEnd_ = false;
	bool tutorialStart_ = false;
	float maxTimer_ = 1.0f;
	float timer_ = 0;
public:
	void Initialize()override;
	void Update()override;
	void OnColliderHit(IFE::Collider* collider)override;
};


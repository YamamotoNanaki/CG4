#pragma once
#include "Component.h"

class TutorialMove : public IFE::Component
{
private:
	IFE::Sprite* LStick_ = nullptr;
	IFE::Sprite* RStick_ = nullptr;
	IFE::Sprite* idou_ = nullptr;
	IFE::Sprite* camera_ = nullptr;
	bool stayPlayer_ = true;
	bool displayEnd_ = false;
	float maxTimer_ = 1.0f;
	float timer_ = 0;
public:
	inline static bool tutorialEnd_ = false;
public:
	void Initialize()override;
	void Update()override;
	void OnColliderHit(IFE::Collider* collider)override;
};


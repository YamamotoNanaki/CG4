#pragma once
#include "Component.h"

enum class LaunchAction
{
	Move, Firework
};
class LaunchFirework : public IFE::Component
{
	using Component::Component;
	float speed_ = 35;
	float timer_ = 0;
	float maxTime_ = 3;
	float targetY_;
	uint8_t phase_ = (uint8_t)LaunchAction::Move;
	IFE::Emitter* bulletEmitter_;
public:
	void Initialize()override;
	void Update()override;
	void SetEmitter(IFE::Emitter* emitter);

};


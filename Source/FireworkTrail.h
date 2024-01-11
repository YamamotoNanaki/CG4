#pragma once
#include "Component.h"
#include "ConstStruct.h"
#include "VertexBUffer.h"

class FireworkTrail : public IFE::Component
{
	uint8_t pattern_ = false;
	inline static uint8_t next_ = false;
	IFE::Emitter* parentPtr_;
	float timer_ = 1;
	const float maxTime_ = 1;
	float endTimer_ = 0;
	const float endTime_ = 3;
public:
	void Initialize()override;
	void Update()override;
	void SetParent(IFE::Emitter* emitter);
};


#include "Player.h"
#include "Input.h"
#include "Transform.h"

using namespace IFE;

void IFE::Player::Update()
{
	Move();
}

void IFE::Player::Move()
{
	Input* input = Input::Instance();
	float x = ((float)input->KeyDown(Key_Rigth) - (float)input->KeyDown(Key_Left)) * speed_;
	float z = ((float)input->KeyDown(Key_Up) - (float)input->KeyDown(Key_Down)) * speed_;
	move_ = { x, 0, z };
	move_.Normalize();
	transform_->position_ += move_;
}

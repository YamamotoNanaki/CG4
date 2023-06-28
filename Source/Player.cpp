#include "Player.h"
#include "Input.h"
#include "Transform.h"
#include "Object3D.h"
#include "Material.h"

using namespace IFE;

void IFE::Player::Update()
{
	Move();
	objectPtr_->GetComponent<Material>()->color_ = { 1,1,1,1 };
}

void IFE::Player::OnColliderHit(Collider* collider)
{
	objectPtr_->GetComponent<Material>()->color_ = { 1,0,0,1 };
	collider;
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

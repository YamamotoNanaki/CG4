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
	float x = ((float)input->KeyDown(Key_Rigth) - (float)input->KeyDown(Key_Left));
	float z = ((float)input->KeyDown(Key_Up) - (float)input->KeyDown(Key_Down));
	move_ = { x, 0, z };
	move_.Normalize();
	transform_->position_ += move_ * speed_;
}



#ifdef _DEBUG
#include "ImguiManager.h"
void IFE::Player::ComponentDebugGUI()
{
	ImguiManager::Instance()->DragFloatGUI(&speed_, "speed", 0.05f);
}

void IFE::Player::OutputComponent(nlohmann::json& json)
{
	json["speed"] = speed_;
}
#endif

void IFE::Player::LoadingComponent(nlohmann::json& json)
{
	speed_ = json["speed"];
}
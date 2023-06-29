#include "Player.h"
#include "Input.h"
#include "Transform.h"
#include "Object3D.h"
#include "Material.h"
#include "CameraManager.h"

using namespace IFE;

void IFE::Player::Update()
{
	Move();
	CameraFollow();
	objectPtr_->GetComponent<Material>()->color_ = { 1,1,1,1 };
}

void IFE::Player::OnColliderHit(Collider* collider)
{
	objectPtr_->GetComponent<Material>()->color_ = { 1,0,0,1 };
	CameraFollow();
	return;
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

void IFE::Player::CameraFollow()
{
	Float3 tar = transform_->position_ + Float3(0, 5, 0);
	Float3 eye = tar + Float3(0, 0, -50);

	Camera* camera = CameraManager::Instance()->sActivCamera_;
	camera->GetView()->eye_ = eye;
	camera->GetView()->target_ = tar;
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
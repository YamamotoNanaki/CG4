#include "Player.h"
#include "Input.h"
#include "Transform.h"
#include "Object3D.h"
#include "Material.h"
#include "CameraManager.h"
#include "ObjectManager.h"
#include "Collider.h"
#include "Enemy.h"
#include "Bullet.h"
#include "IFMath.h"
#include <cmath>

using namespace IFE;

void IFE::Player::Initialize()
{
	moveVec_ = Float2{ 0,-1 };
}

void IFE::Player::Update()
{
	Move();
	Rota();
	Shoot();
	CameraFollow();
	objectPtr_->GetComponent<Material>()->color_ = { 1,1,1,1 };
	transform_->position_ = pos_;
}

void IFE::Player::OnColliderHit(Collider* collider)
{
	if (collider->GetObjectPtr()->GetComponent<Enemy>())
	{
		EnemyCollide();
	}
	else
	{
		return;
	}
	CameraFollow();
	return;
	collider;
}

void IFE::Player::Move()
{
	oldPos_ = pos_;
	Input* input = Input::Instance();
	float x = ((float)input->KeyDown(Key_Rigth) - (float)input->KeyDown(Key_Left));
	float z = ((float)input->KeyDown(Key_Up) - (float)input->KeyDown(Key_Down));
	if (input->GetPadConnected())
	{
		x = input->GetLXAnalog(4000);
		z = input->GetLYAnalog(4000);
	}
	move_ = { x, 0, z };
	move_.Normalize();
	pos_ += move_ * speed_;
}

void IFE::Player::Rota()
{
	Vector2 vec = Vector2{ oldPos_.x - pos_.x,oldPos_.z - pos_.z };
	if (vec == Vector2::zero)return;
	moveVec_ = vec;
	moveVec_.Normalize();
	transform_->eulerAngleDegrees_ = { 0,-ConvertToDegrees(atan2(moveVec_.y , moveVec_.x)) + 90,0 };
}

void IFE::Player::CameraFollow()
{
	Float3 tar = pos_ + Float3(0, 5, 0);
	Float3 eye = tar + Float3(0, 0, -50);

	Camera* camera = CameraManager::Instance()->sActivCamera_;
	camera->GetView()->eye_ = eye;
	camera->GetView()->target_ = tar;
}

void IFE::Player::EnemyCollide()
{
	objectPtr_->GetComponent<Material>()->color_ = { 1,0,0,1 };
}

void IFE::Player::Shoot()
{
	Input* input = Input::Instance();
	if (!input->PadTriggere(PADCODE::RSHOULDER))return;
	Float3 pos = { pos_.x - moveVec_.x * 2,pos_.y,pos_.z - moveVec_.y * 2 };
	auto bullet = ObjectManager::Instance()->Instantiate("Bullet", pos);
	if (bullet == nullptr)return;
	bullet->GetComponent<Bullet>()->SetMoveVector(-moveVec_);
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
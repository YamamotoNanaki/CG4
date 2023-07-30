#include "Goal.h"
#include "Player.h"
#include "Object3D.h"
#include "Input.h"
#include "Scene.h"

void IFE::Goal::Update()
{
	if (goal_ && Input::KeyDown(Key_Space) || Input::PadDown(PADCODE::ABXY))
	{
		Scene::Instance()->SetNextScene("Title");
	}
}

void IFE::Goal::OnColliderHit(Collider* collider)
{
	if (collider->objectPtr_->GetComponent<Player>())
	{
		goal_ = true;
	}
}

#include "EnemySpawn.h"
#include "ObjectManager.h"
#include "Collider.h"
#include "Player.h"

void EnemySpawn::Initialize()
{
	enemys_.push_back(IFE::ObjectManager::Instance()->GetObjectPtr("enemy")->GetComponent<IFE::Enemy>());
	enemys_.push_back(IFE::ObjectManager::Instance()->GetObjectPtr("enemy1")->GetComponent<IFE::Enemy>());
	enemys_.push_back(IFE::ObjectManager::Instance()->GetObjectPtr("enemy2")->GetComponent<IFE::Enemy>());
	enemys_.push_back(IFE::ObjectManager::Instance()->GetObjectPtr("enemy3")->GetComponent<IFE::Enemy>());
	enemys_.push_back(IFE::ObjectManager::Instance()->GetObjectPtr("enemy4")->GetComponent<IFE::Enemy>());
	enemys_.push_back(IFE::ObjectManager::Instance()->GetObjectPtr("enemy5")->GetComponent<IFE::Enemy>());
}

void EnemySpawn::Update()
{
}

void EnemySpawn::OnColliderHit(IFE::Collider* collider)
{
	if (collider->objectPtr_->GetComponent<IFE::Player>())
	{
		if (!once_)
		{
			for (auto itr : enemys_)
			{
				itr->SetLight();
				itr->objectPtr_->isActive_ = true;
			}
			once_ = true;
		}
	}
}

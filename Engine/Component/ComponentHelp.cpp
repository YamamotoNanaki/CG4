#include "ComponentHelp.h"
#include "Component.h"
#include "Transform.h"
#include "TransferGeometryBuffer.h"
#include "Material.h"
#include "CollisionPrimitive.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "ColorBuffer.h"
#include "RectTexture.h"
#include "Fog.h"
#include "Collider.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "TitleUI.h"
#include "TestParticle.h"
#include "BulletParticle.h"
#include "FireworkChrysanthemum.h"
#include "Goal.h"
#include "EnemyCountUI.h"
#include "OverUI.h"
#include "ClearUI.h"
#include "HPbar.h"
#include "PlayerCamera.h"
#include "StartCamera.h"
#include "CommonGameUI.h"
#include "Animator.h"
#include "Boss.h"
#include "EnemyBullet.h"
#include <typeinfo>

using namespace IFE;
using namespace std;

template <class T>
static Component* GetPtr(const string& s);

/*↓をコピペ
tmp = std::move(GetT<クラス名>(str));
if (tmp != nullptr)return tmp;
*/

Component* IFE::StringToComponent(const std::string& str)
{
	Component* tmp = nullptr;
	//↓にコピペ

	tmp = std::move(GetPtr<Transform>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Transform2D>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<TransformParticle>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<TransferGeometryBuffer>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<ColorBuffer>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Material>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Collider>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Enemy>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Bullet>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<EnemyBullet>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<BulletParticle>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<FireworkChrysanthemum>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<TestParticle>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Player>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Animator>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Goal>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<PlayerCamera>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<StartCamera>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<EnemyCountUI>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<TitleUI>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<ClearUI>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<OverUI>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<HPbar>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<CommonGameUI>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<TransformCamera>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<RectTexture>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Fog>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Boss>(str));
	if (tmp != nullptr)return tmp;

	return nullptr;
}

template <class T>
static Component* GetPtr(const string& s)
{
	string n = typeid(T).name();
	if (n.starts_with("class "))
	{
		n.replace(0, 6, "");
	}
	if (n.starts_with("struct "))
	{
		n.replace(0, 7, "");
	}
	if (n.starts_with("IFE::"))
	{
		n.replace(0, 5, "");
	}
	if (s == n)
	{
		T* r = new T;
		r->SetComponentName(n);
		return r;
	}
	return nullptr;
}
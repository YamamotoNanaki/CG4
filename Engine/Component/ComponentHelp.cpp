#include "ComponentHelp.h"
#include "Component.h"
#include "Transform.h"
#include "TransferGeometryBuffer.h"
#include "Material.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "ColorBuffer.h"
#include <typeinfo>

using namespace IFE;
using namespace std;

template <class T>
static Component* GetT(string s);

/*↓をコピペ
tmp = std::move(GetT<クラス名>(str));
if (tmp != nullptr)return tmp;
ここまで*/

Component* IFE::StringToComponent(std::string str)
{
	Component* tmp = nullptr;
	//↓にコピペ


	tmp = std::move(GetT<Transform>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetT<Transform2D>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetT<TransformParticle>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetT<TransferGeometryBuffer>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetT<ColorBuffer>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetT<Material>(str));
	if (tmp != nullptr)return tmp;

	return nullptr;
}

template <class T>
static Component* GetT(string s)
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
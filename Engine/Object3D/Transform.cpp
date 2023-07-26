#include "Transform.h"
#include "ImguiManager.h"
#include "JsonManager.h"
#include "CameraManager.h"
#include "Object3D.h"

using namespace IFE;
using namespace std;

void IFE::Transform::Initialize()
{
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferDataTransform>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
	camera_ = CameraManager::sActivCamera_;
}

void IFE::Transform::Update()
{
}

void IFE::Transform::Draw()
{
	UpdateMatrix();
	View* v = camera_->GetView();
	Projection* p = camera_->GetProjection();
	constMapTransform_->world = matWorld_;
	constMapTransform_->viewPro = v->Get() * p->Get();
	constMapTransform_->cameraPos = v->eye_;
	transformBuffer_->SetConstBuffView(0);
}

void IFE::Transform::UpdateMatrix()
{
	//////ägèk//////
	//ÉXÉPÅ[ÉäÉìÉOî{ó¶ÇçsóÒÇ…ê›íËÇ∑ÇÈ
	matScale_ =
	{ scale_.x,0,0,0,
	0,scale_.y,0,0,
	0,0,scale_.z,0,
	0,0,0,1 };

	//////âÒì]//////

	Float3 eulerRadians = ConvertToRadians(eulerAngleDegrees_);
	rotation_ = EulerToQuaternion(eulerRadians);
	matRot_ = RotateMatrix(rotation_);

	//////ïΩçsà⁄ìÆ//////
	//à⁄ìÆó ÇçsóÒÇ…ê›íËÇ∑ÇÈ
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	position_.x,position_.y,position_.z,1 };


	//íPà çsóÒÇë„ì¸
	matWorld_ = MatrixIdentity();
	//ÉXÉPÅ[ÉäÉìÉOçsóÒÇä|ÇØÇÈ
	matWorld_ *= matScale_;
	//çáê¨ópâÒì]çsóÒÇä|ÇØÇÈ
	matWorld_ *= matRot_;
	//ïΩçsà⁄ìÆçsóÒÇä|ÇØÇÈ
	matWorld_ *= matTrans_;

	if (parent_ == nullptr && objectPtr_->parent_ != nullptr)
	{
		parent_ = objectPtr_->parent_->GetComponent<Transform>();
	}
	if (parent_ != nullptr)
	{
		parent_->UpdateMatrix();
		matWorld_ *= parent_->matWorld_;//êeÇÃçsóÒÇä|ÇØéZÇ∑ÇÈ
		matScale_ *= parent_->matScale_;//êeÇÃÉXÉPÅ[ÉäÉìÉOçsóÒÇ‡ä|ÇØéZÇ∑ÇÈ
		matRot_ *= parent_->matRot_;//êeÇÃâÒì]çsóÒÇ‡ä|ÇØéZÇ∑ÇÈ
		matTrans_ *= parent_->matTrans_;//êeÇÃïΩçsà⁄ìÆçsóÒÇ‡ä|ÇØéZÇ∑ÇÈ
	}
	Matrix ls = matScale_ * matRot_;
	lossyScale_ = { ls.m[0][0],ls.m[1][1],ls.m[2][2] };
}

void IFE::Transform::Copy(Component* component)
{
	Transform* t = dynamic_cast<Transform*>(component);
	if (t == nullptr)return;
	eulerFlag_ = t->eulerFlag_;
	eulerAngleDegrees_ = t->eulerAngleDegrees_;
	billbord_ = t->billbord_;
	scale_ = t->scale_;
	position_ = t->position_;
	rotation_ = t->rotation_;
}

Vector3 IFE::Transform::TransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, matWorld_);
}

Vector3 IFE::Transform::InverseTransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, MatrixInverse(matWorld_));
}

Float3 IFE::Transform::GetLossyScale()
{
	return lossyScale_;
}

Vector3 IFE::Transform::GetForwardVector()
{
	return MultiplyQuaternionAndVector3(rotation_, Vector3(0, 0, 1)).Normalize();
}

Vector3 IFE::Transform::GetUpVector()
{
	return MultiplyQuaternionAndVector3(rotation_, Vector3(0, 1, 0)).Normalize();
}

Vector3 IFE::Transform::GetRightVector()
{
	return MultiplyQuaternionAndVector3(rotation_, Vector3(1, 0, 0)).Normalize();
}

Vector3 Transform::GetWorldPosition()
{
	UpdateMatrix();
	return Matrix::Transform({ 0,0,0 }, matWorld_);
}

void Transform::SetWorldPosition(const Vector3& worldPos)
{
	if (parent_ == nullptr)
	{
		position_ = worldPos;
	}
	else
	{
		position_ = Matrix::Transform(worldPos, MatrixInverse(parent_->matWorld_));
	}
}

IFE::Transform::~Transform()
{
}

#ifdef _DEBUG
void IFE::Transform::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloat3GUI(&position_, "position");
		im->DragFloat3GUI(&eulerAngleDegrees_, "rotation", 1.0f);
		if (eulerAngleDegrees_.x >= 360)
		{
			eulerAngleDegrees_.x -= 360;
		}
		if (eulerAngleDegrees_.y >= 360)
		{
			eulerAngleDegrees_.y -= 360;
		}
		if (eulerAngleDegrees_.z >= 360)
		{
			eulerAngleDegrees_.z -= 360;
		}
		if (eulerAngleDegrees_.x <= 0)
		{
			eulerAngleDegrees_.x += 360;
		}
		if (eulerAngleDegrees_.y <= 0)
		{
			eulerAngleDegrees_.y += 360;
		}
		if (eulerAngleDegrees_.z <= 0)
		{
			eulerAngleDegrees_.z += 360;
		}
		rotation_ = EulerToQuaternion(eulerAngleDegrees_);
		im->DragFloat3GUI(&scale_, "scale");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
}
void IFE::Transform::OutputComponent(nlohmann::json&j)
{
	JsonManager* jm = JsonManager::Instance();
	jm->OutputFloat3(j["scale"], scale_);
	jm->OutputFloat3(j["rotation"], eulerAngleDegrees_);
	jm->OutputFloat3(j["position"], position_);
}
#endif
void IFE::Transform::LoadingComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	scale_ = j->InputFloat3(json["scale"]);
	eulerAngleDegrees_ = j->InputFloat3(json["rotation"]);
	position_ = j->InputFloat3(json["position"]);
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferDataTransform>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
	camera_ = CameraManager::sActivCamera_;
}

#include "WindowsAPI.h"
Matrix Transform2D::matPro_;
void IFE::Transform2D::StaticInitialize()
{
	matPro_ = MatrixOrthoGraphicProjection((float)0, (float)WindowsAPI::Instance()->winWidth_, (float)0, (float)WindowsAPI::Instance()->winHeight_, (float)0, (float)1);
}
void IFE::Transform2D::Initialize()
{
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferMatrix>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
}

void IFE::Transform2D::Update()
{
}

void IFE::Transform2D::Draw()
{
	UpdateMatrix();
	transformBuffer_->SetConstBuffView(1);
}

void IFE::Transform2D::UpdateMatrix()
{
	matWorld_ = MatrixIdentity();
	matWorld_ *= MatrixScaling(scale_.x, scale_.y, 1);
	float r = ConvertToRadians(rotation_);
	matWorld_ *= MatrixRotationZ(r);
	matWorld_ *= MatrixTranslation(position_.x, position_.y, 0);

	//íËêîÉoÉbÉtÉ@Ç÷ÇÃÉfÅ[É^ì]ëó
	constMapTransform_->mat = matWorld_ * matPro_;
}

IFE::Transform2D::~Transform2D()
{
}

#ifdef _DEBUG
void IFE::Transform2D::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloat2GUI(&position_, "position");
		im->DragFloatGUI(&rotation_, "rotation", 1.0f);
		if (rotation_ > 360)
		{
			rotation_ -= 360;
		}
		if (rotation_ < 0)
		{
			rotation_ += 360;
		}
		im->DragFloat2GUI(&scale_, "scale");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
}

void IFE::Transform2D::OutputComponent(nlohmann::json&j)
{
	JsonManager* jm = JsonManager::Instance();
	jm->OutputFloat2(j["scale"], scale_);
	j["rotation"]= rotation_;
	jm->OutputFloat2(j["position"], position_);
}
#endif

void IFE::Transform2D::LoadingComponent(nlohmann::json&json)
{
	JsonManager* j = JsonManager::Instance();
	scale_ = j->InputFloat2(json["scale"]);
	rotation_ = json["rotation"];
	position_ = j->InputFloat2(json["position"]);
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferMatrix>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
}

void IFE::TransformParticle::Initialize()
{
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferBillboard>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
	camera_ = CameraManager::sActivCamera_;
}

void IFE::TransformParticle::Update()
{
}

void IFE::TransformParticle::Draw()
{
	UpdateMatrix();
	View* v = camera_->GetView();
	Projection* p = camera_->GetProjection();
	constMapTransform_->mat = matWorld_ * v->Get() * p->Get();
	constMapTransform_->matBillboard = View::sMatBillBoard_;
	transformBuffer_->SetConstBuffView(1);
}

void IFE::TransformParticle::UpdateMatrix()
{
	//////ägèk//////
	//ÉXÉPÅ[ÉäÉìÉOî{ó¶ÇçsóÒÇ…ê›íËÇ∑ÇÈ
	matScale_ =
	{ scale_.x,0,0,0,
	0,scale_.y,0,0,
	0,0,scale_.z,0,
	0,0,0,1 };

	//////âÒì]//////

	Float3 eulerRadians = ConvertToRadians(eulerAngleDegrees_);
	rotation_ = EulerToQuaternion(eulerRadians);
	matRot_ = RotateMatrix(rotation_);

	//////ïΩçsà⁄ìÆ//////
	//à⁄ìÆó ÇçsóÒÇ…ê›íËÇ∑ÇÈ
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	position_.x,position_.y,position_.z,1 };


	//íPà çsóÒÇë„ì¸
	matWorld_ = MatrixIdentity();
	//ÉXÉPÅ[ÉäÉìÉOçsóÒÇä|ÇØÇÈ
	matWorld_ *= matScale_;
	//çáê¨ópâÒì]çsóÒÇä|ÇØÇÈ
	matWorld_ *= matRot_;
	//ïΩçsà⁄ìÆçsóÒÇä|ÇØÇÈ
	matWorld_ *= matTrans_;

	//if (parent == nullptr && objectPtr->parent != nullptr)
	//{
	//	parent = objectPtr->parent->GetComponent<TransformParticle>();
	//}
	if (parent_ != nullptr)
	{
		parent_->UpdateMatrix();
		matWorld_ *= parent_->matWorld_;//êeÇÃçsóÒÇä|ÇØéZÇ∑ÇÈ
		matScale_ *= parent_->matScale_;//êeÇÃÉXÉPÅ[ÉäÉìÉOçsóÒÇ‡ä|ÇØéZÇ∑ÇÈ
		matRot_ *= parent_->matRot_;//êeÇÃâÒì]çsóÒÇ‡ä|ÇØéZÇ∑ÇÈ
		matTrans_ *= parent_->matTrans_;//êeÇÃïΩçsà⁄ìÆçsóÒÇ‡ä|ÇØéZÇ∑ÇÈ
	}
	Matrix ls = matScale_ * matRot_;
	lossyScale_ = { ls.m[0][0],ls.m[1][1],ls.m[2][2] };
}

Vector3 IFE::TransformParticle::TransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, matWorld_);
}

Vector3 IFE::TransformParticle::InverseTransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, MatrixInverse(matWorld_));
}

Float3 IFE::TransformParticle::GetLossyScale()
{
	return lossyScale_;
}

Vector3 IFE::TransformParticle::GetWorldPosition()
{
	UpdateMatrix();
	return Matrix::Transform({ 0,0,0 }, matWorld_);
}

void IFE::TransformParticle::SetWorldPosition(const Vector3& worldPos)
{
	if (parent_ == nullptr)
	{
		position_ = worldPos;
	}
	else
	{
		position_ = Matrix::Transform(worldPos, MatrixInverse(parent_->matWorld_));
	}
}

IFE::TransformParticle::~TransformParticle()
{
}

void IFE::TransformParticle::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloat3GUI(&position_, "position");
		im->DragFloat3GUI(&eulerAngleDegrees_, "rotation", 1.0f);
		if (eulerAngleDegrees_.x >= 360)
		{
			eulerAngleDegrees_.x -= 360;
		}
		if (eulerAngleDegrees_.y >= 360)
		{
			eulerAngleDegrees_.y -= 360;
		}
		if (eulerAngleDegrees_.z >= 360)
		{
			eulerAngleDegrees_.z -= 360;
		}
		if (eulerAngleDegrees_.x <= 0)
		{
			eulerAngleDegrees_.x += 360;
		}
		if (eulerAngleDegrees_.y <= 0)
		{
			eulerAngleDegrees_.y += 360;
		}
		if (eulerAngleDegrees_.z <= 0)
		{
			eulerAngleDegrees_.z += 360;
		}
		rotation_ = EulerToQuaternion(eulerAngleDegrees_);
		im->DragFloat3GUI(&scale_, "scale");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
}

void IFE::TransformParticle::OutputComponent(nlohmann::json& json)
{
	json;
}

void IFE::TransformParticle::LoadingComponent(nlohmann::json& json)
{
	json;
}

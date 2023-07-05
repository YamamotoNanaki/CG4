#include "Transform.h"
#include "ImguiManager.h"
#include "JsonManager.h"
#include "CameraManager.h"
#include "Object3D.h"

using namespace IFE;
using namespace std;

void IFE::Transform::Initialize()
{
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferTransform>>();
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
	transformBuffer_->SetConstBuffView(4);
}

void IFE::Transform::UpdateMatrix()
{
	//////拡縮//////
	//スケーリング倍率を行列に設定する
	matScale_ =
	{ scale_.x,0,0,0,
	0,scale_.y,0,0,
	0,0,scale_.z,0,
	0,0,0,1 };

	//////回転//////

	//合成用回転行列を宣言し、ZXYの順に合成
	Float3 eulerRadians = ConvertToRadians(eulerAngleDegrees_);
	rotation_ = EulerToQuaternion(eulerRadians);
	matRot_ = RotateMatrix(rotation_);

	//////平行移動//////
	//移動量を行列に設定する
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	position_.x,position_.y,position_.z,1 };


	//単位行列を代入
	matWorld_ = MatrixIdentity();
	//スケーリング行列を掛ける
	matWorld_ *= matScale_;
	//合成用回転行列を掛ける
	matWorld_ *= matRot_;
	//平行移動行列を掛ける
	matWorld_ *= matTrans_;

	if (parent_ == nullptr && objectPtr_->parent_ != nullptr)
	{
		parent_ = objectPtr_->parent_->GetComponent<Transform>();
	}
	if (parent_ != nullptr)
	{
		parent_->UpdateMatrix();
		matWorld_ *= parent_->matWorld_;//親の行列を掛け算する
		matScale_ *= parent_->matScale_;//親のスケーリング行列も掛け算する
		matRot_ *= parent_->matRot_;//親の回転行列も掛け算する
		matTrans_ *= parent_->matTrans_;//親の平行移動行列も掛け算する
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
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferTransform>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
	camera_ = CameraManager::sActivCamera_;
}

#include "WindowsAPI.h"
void IFE::Transform2D::Initialize()
{
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferTransform>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
}

void IFE::Transform2D::Update()
{
}

void IFE::Transform2D::Draw()
{
	UpdateMatrix();
	transformBuffer_->SetConstBuffView(4);
}

void IFE::Transform2D::UpdateMatrix()
{
	matWorld_ = MatrixIdentity();
	matWorld_ *= MatrixScaling(scale_.x, scale_.y, 1);
	float r = ConvertToRadians(rotation_);
	matWorld_ *= MatrixRotationZ(r);
	matWorld_ *= MatrixTranslation(position_.x, position_.y, 0);

	//定数バッファへのデータ転送
	constMapTransform_->world = matWorld_;
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
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferTransform>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
}

void IFE::TransformParticle::Initialize()
{
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferTransform>>();
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
	constMapTransform_->world = matWorld_;
	transformBuffer_->SetConstBuffView(1);
}

void IFE::TransformParticle::UpdateMatrix()
{
	//////拡縮//////
	//スケーリング倍率を行列に設定する
	matScale_ =
	{ scale_.x,0,0,0,
	0,scale_.y,0,0,
	0,0,scale_.z,0,
	0,0,0,1 };

	//////回転//////

	//合成用回転行列を宣言し、ZXYの順に合成
	float r = ConvertToRadians(rotation_);
	matRot_ *= MatrixRotationZ(r);

	//////平行移動//////
	//移動量を行列に設定する
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	position_.x,position_.y,position_.z,1 };


	//単位行列を代入
	matWorld_ = MatrixIdentity();
	//スケーリング行列を掛ける
	matWorld_ *= matScale_;
	//合成用回転行列を掛ける
	matWorld_ *= matRot_;
	//平行移動行列を掛ける
	matWorld_ *= matTrans_;

	//if (parent == nullptr && objectPtr->parent != nullptr)
	//{
	//	parent = objectPtr->parent->GetComponent<TransformParticle>();
	//}
	if (parent_ != nullptr)
	{
		parent_->UpdateMatrix();
		matWorld_ *= parent_->matWorld_;//親の行列を掛け算する
		matScale_ *= parent_->matScale_;//親のスケーリング行列も掛け算する
		matRot_ *= parent_->matRot_;//親の回転行列も掛け算する
		matTrans_ *= parent_->matTrans_;//親の平行移動行列も掛け算する
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

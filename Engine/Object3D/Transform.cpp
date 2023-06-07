#include "Transform.h"
#include "ImguiManager.h"
#include "JsonManager.h"
#include "CameraManager.h"
#include "Object3D.h"

using namespace IFE;
using namespace std;

void IFE::Transform::Initialize()
{
	transformBuffer = make_unique<ConstBuffer<ConstBufferDataTransform>>();
	constMapTransform = transformBuffer->GetCBMapObject();
	camera = CameraManager::activCamera;
}

void IFE::Transform::Update()
{
}

void IFE::Transform::Draw()
{
	UpdateMatrix();
	View* v = camera->GetView();
	Projection* p = camera->GetProjection();
	constMapTransform->world = matWorld;
	constMapTransform->viewPro = v->Get() * p->Get();
	constMapTransform->cameraPos = v->eye;
	transformBuffer->SetConstBuffView(0);
}

void IFE::Transform::UpdateMatrix()
{
	//////拡縮//////
	//スケーリング倍率を行列に設定する
	matScale_ =
	{ scale.x,0,0,0,
	0,scale.y,0,0,
	0,0,scale.z,0,
	0,0,0,1 };

	//////回転//////

	//合成用回転行列を宣言し、ZXYの順に合成
	Float3 eulerRadians = ConvertToRadians(eulerAngleDegrees);
	rotation = EulerToQuaternion(eulerRadians);
	matRot_ = RotateMatrix(rotation);

	//////平行移動//////
	//移動量を行列に設定する
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	position.x,position.y,position.z,1 };


	//単位行列を代入
	matWorld = MatrixIdentity();
	//スケーリング行列を掛ける
	matWorld *= matScale_;
	//合成用回転行列を掛ける
	matWorld *= matRot_;
	//平行移動行列を掛ける
	matWorld *= matTrans_;

	if (parent == nullptr && objectPtr->parent != nullptr)
	{
		parent = objectPtr->parent->GetComponent<Transform>();
	}
	if (parent != nullptr)
	{
		parent->UpdateMatrix();
		matWorld *= parent->matWorld;//親の行列を掛け算する
		matScale_ *= parent->matScale_;//親のスケーリング行列も掛け算する
		matRot_ *= parent->matRot_;//親の回転行列も掛け算する
		matTrans_ *= parent->matTrans_;//親の平行移動行列も掛け算する
	}
	Matrix ls = matScale_ * matRot_;
	lossyScale = { ls.m[0][0],ls.m[1][1],ls.m[2][2] };
}

void IFE::Transform::Copy(Component* component)
{
	Transform* t = dynamic_cast<Transform*>(component);
	if (t == nullptr)return;
	eulerFlag = t->eulerFlag;
	eulerAngleDegrees = t->eulerAngleDegrees;
	billbord = t->billbord;
	scale = t->scale;
	position = t->position;
	rotation = t->rotation;
}

Vector3 IFE::Transform::TransformPoint(Vector3 p)
{
	UpdateMatrix();
	return Matrix::Transform(p, matWorld);
}

Vector3 IFE::Transform::InverseTransformPoint(Vector3 p)
{
	UpdateMatrix();
	return Matrix::Transform(p, MatrixInverse(matWorld));
}

Float3 IFE::Transform::GetLossyScale()
{
	return lossyScale;
}

Vector3 IFE::Transform::GetForwardVector()
{
	return MultiplyQuaternionAndVector3(rotation, Vector3(0, 0, 1)).Normalize();
}

Vector3 IFE::Transform::GetUpVector()
{
	return MultiplyQuaternionAndVector3(rotation, Vector3(0, 1, 0)).Normalize();
}

Vector3 IFE::Transform::GetRightVector()
{
	return MultiplyQuaternionAndVector3(rotation, Vector3(1, 0, 0)).Normalize();
}

Vector3 Transform::GetWorldPosition()
{
	UpdateMatrix();
	return Matrix::Transform({ 0,0,0 }, matWorld);
}

void Transform::SetWorldPosition(Vector3 worldPos)
{
	if (parent == nullptr)
	{
		position = worldPos;
	}
	else
	{
		position = Matrix::Transform(worldPos, MatrixInverse(parent->matWorld));
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
		im->DragFloat3GUI(&position, "position");
		im->DragFloat3GUI(&eulerAngleDegrees, "rotation", 1.0f);
		if (eulerAngleDegrees.x >= 360)
		{
			eulerAngleDegrees.x -= 360;
		}
		if (eulerAngleDegrees.y >= 360)
		{
			eulerAngleDegrees.y -= 360;
		}
		if (eulerAngleDegrees.z >= 360)
		{
			eulerAngleDegrees.z -= 360;
		}
		if (eulerAngleDegrees.x <= 0)
		{
			eulerAngleDegrees.x += 360;
		}
		if (eulerAngleDegrees.y <= 0)
		{
			eulerAngleDegrees.y += 360;
		}
		if (eulerAngleDegrees.z <= 0)
		{
			eulerAngleDegrees.z += 360;
		}
		rotation = EulerToQuaternion(eulerAngleDegrees);
		im->DragFloat3GUI(&scale, "scale");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName);
}
void IFE::Transform::OutputComponent()
{
	JsonManager* j = JsonManager::Instance();
	j->OutputFloat3("scale", scale);
	j->OutputFloat3("rotation", eulerAngleDegrees);
	j->OutputFloat3("position", position);
}
#endif
void IFE::Transform::LoadingComponent()
{
	JsonManager* j = JsonManager::Instance();
	scale = j->InputFloat3("scale");
	eulerAngleDegrees = j->InputFloat3("rotation");
	position = j->InputFloat3("position");
	transformBuffer = make_unique<ConstBuffer<ConstBufferDataTransform>>();
	constMapTransform = transformBuffer->GetCBMapObject();
	camera = CameraManager::activCamera;
}

#include "WindowsAPI.h"
Matrix Transform2D::matPro;
void IFE::Transform2D::StaticInitialize()
{
	matPro = MatrixOrthoGraphicProjection((float)0, (float)WindowsAPI::Instance()->winWidth, (float)0, (float)WindowsAPI::Instance()->winHeight, (float)0, (float)1);
}
void IFE::Transform2D::Initialize()
{
	transformBuffer = make_unique<ConstBuffer<ConstBufferMatrix>>();
	constMapTransform = transformBuffer->GetCBMapObject();
}

void IFE::Transform2D::Update()
{
}

void IFE::Transform2D::Draw()
{
	UpdateMatrix();
	transformBuffer->SetConstBuffView(1);
}

void IFE::Transform2D::UpdateMatrix()
{
	matWorld = MatrixIdentity();
	matWorld *= MatrixScaling(scale.x, scale.y, 1);
	float r = ConvertToRadians(rotation);
	matWorld *= MatrixRotationZ(r);
	matWorld *= MatrixTranslation(position.x, position.y, 0);

	//定数バッファへのデータ転送
	constMapTransform->mat = matWorld * matPro;
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
		im->DragFloat2GUI(&position, "position");
		im->DragFloatGUI(&rotation, "rotation", 1.0f);
		if (rotation > 360)
		{
			rotation -= 360;
		}
		if (rotation < 0)
		{
			rotation += 360;
		}
		im->DragFloat2GUI(&scale, "scale");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName);
}

void IFE::Transform2D::OutputComponent()
{
	JsonManager* j = JsonManager::Instance();
	j->OutputFloat2("scale", scale);
	j->OutputFloat("rotation", rotation);
	j->OutputFloat2("position", position);
}
#endif

void IFE::Transform2D::LoadingComponent()
{
	JsonManager* j = JsonManager::Instance();
	scale = j->InputFloat2("scale");
	rotation = j->InputFloat("rotation");
	position = j->InputFloat2("position");
	transformBuffer = make_unique<ConstBuffer<ConstBufferMatrix>>();
	constMapTransform = transformBuffer->GetCBMapObject();
}

void IFE::TransformParticle::Initialize()
{
	transformBuffer = make_unique<ConstBuffer<ConstBufferBillboard>>();
	constMapTransform = transformBuffer->GetCBMapObject();
	camera = CameraManager::activCamera;
}

void IFE::TransformParticle::Update()
{
}

void IFE::TransformParticle::Draw()
{
	UpdateMatrix();
	View* v = camera->GetView();
	Projection* p = camera->GetProjection();
	constMapTransform->mat = matWorld * v->Get() * p->Get();
	constMapTransform->matBillboard = View::matBillBoard;
	transformBuffer->SetConstBuffView(1);
}

void IFE::TransformParticle::UpdateMatrix()
{
	//////拡縮//////
	//スケーリング倍率を行列に設定する
	matScale_ =
	{ scale.x,0,0,0,
	0,scale.y,0,0,
	0,0,scale.z,0,
	0,0,0,1 };

	//////回転//////

	//合成用回転行列を宣言し、ZXYの順に合成
	float r = ConvertToRadians(rotation);
	matRot_ *= MatrixRotationZ(r);

	//////平行移動//////
	//移動量を行列に設定する
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	position.x,position.y,position.z,1 };


	//単位行列を代入
	matWorld = MatrixIdentity();
	//スケーリング行列を掛ける
	matWorld *= matScale_;
	//合成用回転行列を掛ける
	matWorld *= matRot_;
	//平行移動行列を掛ける
	matWorld *= matTrans_;

	//if (parent == nullptr && objectPtr->parent != nullptr)
	//{
	//	parent = objectPtr->parent->GetComponent<TransformParticle>();
	//}
	if (parent != nullptr)
	{
		parent->UpdateMatrix();
		matWorld *= parent->matWorld;//親の行列を掛け算する
		matScale_ *= parent->matScale_;//親のスケーリング行列も掛け算する
		matRot_ *= parent->matRot_;//親の回転行列も掛け算する
		matTrans_ *= parent->matTrans_;//親の平行移動行列も掛け算する
	}
	Matrix ls = matScale_ * matRot_;
	lossyScale = { ls.m[0][0],ls.m[1][1],ls.m[2][2] };
}

Vector3 IFE::TransformParticle::TransformPoint(Vector3 p)
{
	UpdateMatrix();
	return Matrix::Transform(p, matWorld);
}

Vector3 IFE::TransformParticle::InverseTransformPoint(Vector3 p)
{
	UpdateMatrix();
	return Matrix::Transform(p, MatrixInverse(matWorld));
}

Float3 IFE::TransformParticle::GetLossyScale()
{
	return lossyScale;
}

Vector3 IFE::TransformParticle::GetWorldPosition()
{
	UpdateMatrix();
	return Matrix::Transform({ 0,0,0 }, matWorld);
}

void IFE::TransformParticle::SetWorldPosition(Vector3 worldPos)
{
	if (parent == nullptr)
	{
		position = worldPos;
	}
	else
	{
		position = Matrix::Transform(worldPos, MatrixInverse(parent->matWorld));
	}
}

IFE::TransformParticle::~TransformParticle()
{
}

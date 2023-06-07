#include "ComponentManager.h"
#include "Object3D.h"

void IFE::ComponentManager::Initialize()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->INITIALIZE();
	}
}

void IFE::ComponentManager::Update()
{
	componentList.remove_if([](Component* com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList)
	{
		if (itr)itr->Update();
	}
}

void IFE::ComponentManager::Draw()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->Draw();
	}
}

void IFE::ComponentManager::Finalize()
{
	for (auto& itr : componentList)
	{
		if (itr->GetComponentName().find("_Model") != std::string::npos)
		{
			continue;
		}
		if (itr)delete itr;
	}
	componentList.clear();
}

void IFE::ComponentManager::SetObjectPtr(Object3D* p)
{
	objectPtr = p;
}

//void IFE::ComponentManager::OnColliderHit(ADXCollider* myCol, ADXCollider* col)
//{
//	for (auto& itr : componentList)
//	{
//		itr->OnColliderHit(myCol, col);
//	}
//}

std::vector<std::string> IFE::ComponentManager::GetAllComponentName()
{
	std::vector<std::string>s;

	for (auto& itr : componentList)
	{
		s.push_back(itr->GetComponentName());
	}
	return s;
}

#ifdef _DEBUG
void IFE::ComponentManager::DebugGUI()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->DebugGUI();
	}
}

void IFE::ComponentManager::OutputScene(std::string object3D)
{
	for (auto& itr : componentList)
	{
		if (itr)itr->OutputScene(object3D);
	}
}

void IFE::ComponentManager::DebugUpdate()
{
	componentList.remove_if([](Component* com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList)
	{
		std::string name = itr->GetComponentName();
		if (name == "Transform");
		else if (name == "TransferGeometryBuffer");
		else if (name == "Material");
		else if (name.find("_Model") != std::string::npos);
		else
		{
			continue;
		}
		if (itr)itr->Update();
	}
}
#endif

void IFE::ComponentManager::LoadingScene(std::string object3D, std::string componentName)
{
	for (auto& itr : componentList)
	{
		if (itr)itr->LoadingScene(object3D, componentName);
	}
}

std::string IFE::ComponentManager::SetName(std::string name)
{
	if (name.starts_with("class "))
	{
		name.replace(0, 6, "");
	}
	if (name.starts_with("struct "))
	{
		name.replace(0, 7, "");
	}
	if (name.starts_with("IFE::"))
	{
		name.replace(0, 5, "");
	}
	return name;
}

void IFE::ComponentManager2D::Initialize()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->INITIALIZE();
	}
}

void IFE::ComponentManager2D::Update()
{
	componentList.remove_if([](Component* com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList)
	{
		if (itr)itr->Update();
	}
}

void IFE::ComponentManager2D::Draw()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->Draw();
	}
}

void IFE::ComponentManager2D::Finalize()
{
	for (auto& itr : componentList)
	{
		if (itr->GetComponentName().find("_Model") != std::string::npos)
		{
			continue;
		}
		if (itr)delete itr;
	}
	componentList.clear();
}

void IFE::ComponentManager2D::SetSpritePtr(Sprite* p)
{
	spritePtr = p;
}

std::vector<std::string> IFE::ComponentManager2D::GetAllComponentName()
{
	std::vector<std::string>s;

	for (auto& itr : componentList)
	{
		s.push_back(itr->GetComponentName());
	}
	return s;
}

#ifdef _DEBUG
void IFE::ComponentManager2D::DebugGUI()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->DebugGUI();
	}
}

void IFE::ComponentManager2D::OutputScene(std::string sprite)
{
	for (auto& itr : componentList)
	{
		if (itr)itr->OutputScene(sprite);
	}
}

void IFE::ComponentManager2D::DebugUpdate()
{
	componentList.remove_if([](Component* com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList)
	{
		std::string name = itr->GetComponentName();
		if (name == "Transform2D");
		else if (name == "ColorBuffer");
		//else if (name == "Material");
		//else if (name.find("_Model") != std::string::npos);
		else
		{
			continue;
		}
		if (itr)itr->Update();
	}
}
#endif

void IFE::ComponentManager2D::LoadingScene(std::string sprite, std::string componentName)
{
	for (auto& itr : componentList)
	{
		if (itr)itr->LoadingScene(sprite, componentName);
	}
}

std::string IFE::ComponentManager2D::SetName(std::string name)
{
	if (name.starts_with("class "))
	{
		name.replace(0, 6, "");
	}
	if (name.starts_with("struct "))
	{
		name.replace(0, 7, "");
	}
	if (name.starts_with("IFE::"))
	{
		name.replace(0, 5, "");
	}
	return name;
}

void IFE::ComponentManagerParticle::Initialize()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->INITIALIZE();
	}
}

void IFE::ComponentManagerParticle::Update()
{
	componentList.remove_if([](Component* com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList)
	{
		if (itr)itr->Update();
	}
}

void IFE::ComponentManagerParticle::Draw()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->Draw();
	}
}

void IFE::ComponentManagerParticle::Finalize()
{
	for (auto& itr : componentList)
	{
		if (itr->GetComponentName().find("_Model") != std::string::npos)
		{
			continue;
		}
		if (itr)delete itr;
	}
	componentList.clear();
}

void IFE::ComponentManagerParticle::SetParticlePtr(Particle* p)
{
	particlePtr = p;
}

std::vector<std::string> IFE::ComponentManagerParticle::GetAllComponentName()
{
	std::vector<std::string>s;

	for (auto& itr : componentList)
	{
		s.push_back(itr->GetComponentName());
	}
	return s;
}

#ifdef _DEBUG
void IFE::ComponentManagerParticle::DebugGUI()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->DebugGUI();
	}
}

void IFE::ComponentManagerParticle::OutputScene(std::string particle)
{
	for (auto& itr : componentList)
	{
		if (itr)itr->OutputScene(particle);
	}
}

void IFE::ComponentManagerParticle::DebugUpdate()
{
	componentList.remove_if([](Component* com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList)
	{
		std::string name = itr->GetComponentName();
		if (name == "Transform2D");
		else if (name == "ColorBuffer");
		//else if (name == "Material");
		//else if (name.find("_Model") != std::string::npos);
		else
		{
			continue;
		}
		if (itr)itr->Update();
	}
}
#endif

void IFE::ComponentManagerParticle::LoadingScene(std::string particle, std::string componentName)
{
	for (auto& itr : componentList)
	{
		if (itr)itr->LoadingScene(particle, componentName);
	}
}

std::string IFE::ComponentManagerParticle::SetName(std::string name)
{
	if (name.starts_with("class "))
	{
		name.replace(0, 6, "");
	}
	if (name.starts_with("struct "))
	{
		name.replace(0, 7, "");
	}
	if (name.starts_with("IFE::"))
	{
		name.replace(0, 5, "");
	}
	return name;
}

void IFE::ComponentManagerEmitter::Initialize()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->INITIALIZE();
	}
}

void IFE::ComponentManagerEmitter::Update()
{
	componentList.remove_if([](Component* com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList)
	{
		if (itr)itr->Update();
	}
}

void IFE::ComponentManagerEmitter::Draw()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->Draw();
	}
}

void IFE::ComponentManagerEmitter::Finalize()
{
	for (auto& itr : componentList)
	{
		if (itr->GetComponentName().find("_Model") != std::string::npos)
		{
			continue;
		}
		if (itr)delete itr;
	}
	componentList.clear();
}

void IFE::ComponentManagerEmitter::SetEmitterPtr(Emitter* e)
{
	emitterPtr = e;
}

std::vector<std::string> IFE::ComponentManagerEmitter::GetAllComponentName()
{
	std::vector<std::string>s;

	for (auto& itr : componentList)
	{
		s.push_back(itr->GetComponentName());
	}
	return s;
}

#ifdef _DEBUG
void IFE::ComponentManagerEmitter::DebugGUI()
{
	for (auto& itr : componentList)
	{
		if (itr)itr->DebugGUI();
	}
}

void IFE::ComponentManagerEmitter::OutputScene(std::string emitter)
{
	for (auto& itr : componentList)
	{
		if (itr)itr->OutputScene(emitter);
	}
}

void IFE::ComponentManagerEmitter::DebugUpdate()
{
	componentList.remove_if([](Component* com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList)
	{
		std::string name = itr->GetComponentName();
		if (name == "Transform2D");
		else if (name == "ColorBuffer");
		//else if (name == "Material");
		//else if (name.find("_Model") != std::string::npos);
		else
		{
			continue;
		}
		if (itr)itr->Update();
	}
}
#endif

void IFE::ComponentManagerEmitter::LoadingScene(std::string emitter, std::string componentName)
{
	for (auto& itr : componentList)
	{
		if (itr)itr->LoadingScene(emitter, componentName);
	}
}

std::string IFE::ComponentManagerEmitter::SetName(std::string name)
{
	if (name.starts_with("class "))
	{
		name.replace(0, 6, "");
	}
	if (name.starts_with("struct "))
	{
		name.replace(0, 7, "");
	}
	if (name.starts_with("IFE::"))
	{
		name.replace(0, 5, "");
	}
	return name;
}

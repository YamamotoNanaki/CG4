#pragma once
#include "Particle.h"
#include "Texture.h"
#include "GraphicsPipeline.h"
#include <memory>
#include <list>
#include <string>
//#include "PostEffect.h"

namespace IFE
{
	class TransformParticle;
	class Emitter :public ComponentManager
	{

	public:
		std::list<std::unique_ptr<Particle>>particles_;
		bool deleteFlag_ = false;
		bool emitterDeleteFlag_ = false;
		TransformParticle* transform_ = nullptr;
		bool isActive_ = true;
		bool DrawFlag_ = true;
		Texture* tex_ = nullptr;
		std::string emitterName_;
		GraphicsPipeline* gp_;
		float particleMaxTime_ = 1;

	public:
		//PostEffect* postEffect = nullptr;

	public:
		Emitter();
		void Initialize();
		static void StaticInitialize();
		void SetComponent(std::unique_ptr<Component> component);
		void SetComponentFront(std::unique_ptr<Component> component);
		void SetTexture(const std::string& texName);
		Particle* AddParticle(bool parentFlag);
		Particle* AddParticle(Float3 pos);
		Particle* AddParticle();
		void Update();
		//static void DrawBefore();
		void Draw();
		~Emitter();

#ifdef _DEBUG
		void DebugGUI(bool flagdelete);
		void ComponentGUI();
		void OutputScene();
		void DebugUpdate();
#endif
	};
}

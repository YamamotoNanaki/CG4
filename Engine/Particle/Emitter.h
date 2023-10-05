#pragma once
#include "Particle.h"
#include "Texture.h"
#include "GraphicsPipeline.h"
#include "Warning.h"
WarningDisabled
#include <nlohmann/json.hpp>
#include <memory>
#include <list>
#include <string>
WarningDisabledPop
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
		void CopyValue(Emitter* ptr);
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

#ifdef NDEBUG
#else
		void DebugGUI(bool flagdelete);
		void ComponentGUI();
		void OutputScene(nlohmann::json& j);
		void DebugUpdate();
#endif
		void LoadingScene(nlohmann::json& j);
	};
}

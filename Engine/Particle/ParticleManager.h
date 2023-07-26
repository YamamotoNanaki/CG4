#pragma once
#include <list>
#include <memory>
#include "Emitter.h"
#include "Texture.h"
#include "IFMath.h"

namespace IFE
{
	class ParticleManager
	{
		std::list<std::unique_ptr<Emitter>>emitterList_;
	private:
		ParticleManager() {}
		ParticleManager(const ParticleManager&);
		ParticleManager& operator=(const ParticleManager&) {}
		~ParticleManager()
		{
			emitterList_.clear();
		}

	public:
		static ParticleManager* Instance();
		static void Finalize();
		void Initialize();
		void Update();
		void Draw();
		void AddInitialize(const std::string& emitterName, const std::string& textureName);
		Emitter* Add(const std::string& emitterName);
		Emitter* GetEmitterPtr(const std::string& emitterName);
		Emitter* Instantiate(Emitter* gameObject, const Float3& position, float rotation);
		Emitter* Instantiate(Emitter* gameObject, const Float3& position);
		Emitter* Instantiate(Emitter* gameObject);
		void SetTexture(const std::string& emitterName, const std::string& texName);
		std::list<std::unique_ptr<Emitter>>* GetEmitterList();
		void Reset();
		bool SearchName(const std::string& name);

#ifdef _DEBUG
		void DebugGUI();
		void OutputScene();
		void DebugUpdate();
#endif
		void LoadingScene();

	private:
		std::string GetNewName(const std::string& emitterName);
	};
}

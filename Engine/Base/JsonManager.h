#pragma once
#include <string>
#include "IFMath.h"
#include "nlohmann/json.hpp"

namespace IFE
{
	class JsonManager
	{
		JsonManager() {}
		JsonManager(const JsonManager&) {}
		JsonManager& operator=(const JsonManager&) {}
		~JsonManager() {}
		nlohmann::json _json;

		std::string sceneName;

	public:
		void SetSceneName(std::string sceneName);
		static JsonManager* Instance();
		float InputFloat(std::string jsonName);
		int32_t InputInt(std::string jsonName);
		bool InputBool(std::string jsonName);
		uint32_t InputUINT(std::string jsonName);
		std::string InputString(std::string jsonName);
		Float2 InputFloat2(std::string jsonName);
		Float3 InputFloat3(std::string jsonName);
		Float4 InputFloat4(std::string jsonName);

		void Input(std::string filename);
		void JsonReset();
		std::string SceneInit();

		nlohmann::json& GetJsonData();

		bool error = false;

#ifdef _DEBUG
		void OutputFloat(std::string jsonName, float f);
		void OutputInt(std::string jsonName, std::int32_t i);
		void OutputBool(std::string jsonName, bool b);
		void OutputUINT(std::string jsonName, uint32_t i);
		void OutputString(std::string jsonName, std::string str);
		void OutputFloat2(std::string jsonName, Float2 f);
		void OutputFloat3(std::string jsonName, Float3 f);
		void OutputFloat4(std::string jsonName, Float4 f);
		void OutputFloat2(std::string jsonName, Vector2 v);
		void OutputFloat3(std::string jsonName, Vector3 v);

		//void Output(std::string filename);
		void OutputAndMakeDirectry(std::string filename, std::string directry);
		void MakeDirectry(std::string directry);
		void MakeSceneDirectry();
		void SetInitScene();
#endif
	};
}

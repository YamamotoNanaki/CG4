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
		nlohmann::json json_;

		std::string sceneName_;

	public:
		void SetSceneName(const std::string& sceneName);
		static JsonManager* Instance();
		float InputFloat(const std::string& jsonName);
		int32_t InputInt(const std::string& jsonName);
		bool InputBool(const std::string& jsonName);
		uint32_t InputUINT(const std::string& jsonName);
		std::string InputString(const std::string& jsonName);
		Float2 InputFloat2(const std::string& jsonName);
		Float3 InputFloat3(const std::string& jsonName);
		Float4 InputFloat4(const std::string& jsonName);

		void Input(const std::string& filename);
		void JsonReset();
		std::string SceneInit();

		nlohmann::json& GetJsonData();

		bool error_ = false;

#ifdef _DEBUG
		void OutputFloat(const std::string& jsonName, float f);
		void OutputInt(const std::string& jsonName, std::int32_t i);
		void OutputBool(const std::string& jsonName, bool b);
		void OutputUINT(const std::string& jsonName, uint32_t i);
		void OutputString(const std::string& jsonName, const std::string& str);
		void OutputFloat2(const std::string& jsonName, const Float2& f);
		void OutputFloat3(const std::string& jsonName, const Float3& f);
		void OutputFloat4(const std::string& jsonName, const Float4& f);
		void OutputFloat2(const std::string& jsonName, const Vector2& v);
		void OutputFloat3(const std::string& jsonName, const Vector3& v);

		//void Output(std::string filename);
		void OutputAndMakeDirectry(const std::string& filename, const std::string& directry);
		void MakeDirectry(const std::string& directry);
		void MakeSceneDirectry();
		void SetInitScene();
#endif
	};
}

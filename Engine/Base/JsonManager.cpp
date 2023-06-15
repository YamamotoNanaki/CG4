#include "JsonManager.h"
#include <iostream>
#include <fstream>

using namespace IFE;
using namespace std;

void IFE::JsonManager::SetSceneName(const std::string& s)
{
	sceneName_ = s;
}

JsonManager* IFE::JsonManager::Instance()
{
	static JsonManager inst;
	return &inst;
}

float IFE::JsonManager::InputFloat(const std::string& jsonName)
{
	return json_[jsonName];
}

std::int32_t IFE::JsonManager::InputInt(const std::string& jsonName)
{
	return json_[jsonName];
}

bool IFE::JsonManager::InputBool(const std::string& jsonName)
{
	return json_[jsonName];
}

uint32_t IFE::JsonManager::InputUINT(const std::string& jsonName)
{
	return json_[jsonName];
}

std::string IFE::JsonManager::InputString(const std::string& jsonName)
{
	return json_[jsonName];
}

Float2 IFE::JsonManager::InputFloat2(const std::string& jsonName)
{
	return Float2(json_[jsonName][0], json_[jsonName][1]);
}

Float3 IFE::JsonManager::InputFloat3(const std::string& jsonName)
{
	return Float3(json_[jsonName][0], json_[jsonName][1], json_[jsonName][2]);
}

Float4 IFE::JsonManager::InputFloat4(const std::string& jsonName)
{
	return Float4(json_[jsonName][0], json_[jsonName][1], json_[jsonName][2], json_[jsonName][3]);
}

void IFE::JsonManager::Input(const std::string& filename)
{
	JsonReset();
	string name = "Data/Scene/" + sceneName_ + "/" + filename + ".json";
	ifstream readingFile;
	readingFile.open(name, std::ios::in);
	if (!readingFile) {
		readingFile.close();
		error_ = true;
		return;
	}
	readingFile >> json_;
	readingFile.close();
}

void IFE::JsonManager::JsonReset()
{
	nlohmann::json j;
	json_ = j;
}

std::string IFE::JsonManager::SceneInit()
{
	JsonReset();
	string name = "Data/Scene/SceneManager.json";
	ifstream readingFile;
	readingFile.open(name, std::ios::in);
	if (!readingFile) {
		readingFile.close();
		error_ = true;
		return "";
	}
	readingFile >> json_;
	readingFile.close();
	return json_["start"];
}

nlohmann::json& IFE::JsonManager::GetJsonData()
{
	return json_;
}


#ifdef _DEBUG
void IFE::JsonManager::OutputFloat(const std::string& jsonName, float f)
{
	json_[jsonName] = f;
}

void IFE::JsonManager::OutputInt(const std::string& jsonName, int32_t i)
{
	json_[jsonName] = i;
}

void IFE::JsonManager::OutputBool(const std::string& jsonName, bool b)
{
	json_[jsonName] = b;
}

void IFE::JsonManager::OutputUINT(const std::string& jsonName, uint32_t i)
{
	json_[jsonName] = i;
}

void IFE::JsonManager::OutputString(const std::string& jsonName, const std::string& str)
{
	json_[jsonName] = str;
}

void IFE::JsonManager::OutputFloat2(const std::string& jsonName, const Float2& f)
{
	json_[jsonName][0] = f.x;
	json_[jsonName][1] = f.y;
}

void IFE::JsonManager::OutputFloat3(const std::string& jsonName, const Float3& f)
{
	json_[jsonName][0] = f.x;
	json_[jsonName][1] = f.y;
	json_[jsonName][2] = f.z;
}

void IFE::JsonManager::OutputFloat4(const std::string& jsonName, const Float4& f)
{
	json_[jsonName][0] = f.x;
	json_[jsonName][1] = f.y;
	json_[jsonName][2] = f.z;
	json_[jsonName][3] = f.w;
}

void IFE::JsonManager::OutputFloat2(const std::string& jsonName, const Vector2& v)
{
	json_[jsonName][0] = v.x;
	json_[jsonName][1] = v.y;
}

void IFE::JsonManager::OutputFloat3(const std::string& jsonName, const Vector3& v)
{
	json_[jsonName][0] = v.x;
	json_[jsonName][1] = v.y;
	json_[jsonName][2] = v.z;
}

//void IFE::JsonManager::Output(std::string filename)
//{
//	string s = _json.dump(4);
//	ofstream writingFile;
//	string name = "Data/Scene/" + filename + ".json";
//	writingFile.open(name, std::ios::out);
//	writingFile << s << std::endl;
//	writingFile.close();
//	JsonReset();
//}

#include <direct.h>
void IFE::JsonManager::OutputAndMakeDirectry(const std::string& filename, const std::string& directry)
{
	string dir = "Data/Scene/" + sceneName_ + "/" + directry;
	_mkdir(dir.c_str());
	string s = json_.dump(4);
	ofstream writingFile;
	string name = dir + "/" + filename + ".json";
	writingFile.open(name, std::ios::out);
	writingFile << s << std::endl;
	writingFile.close();
	JsonReset();
}

void IFE::JsonManager::MakeDirectry(const std::string& directry)
{
	string dir = "Data/Scene/" + sceneName_ + "/" + directry;
	_mkdir(dir.c_str());
}

void IFE::JsonManager::MakeSceneDirectry()
{
	string dir = "Data/Scene/" + sceneName_;
	_mkdir(dir.c_str());
}

void IFE::JsonManager::SetInitScene()
{
	JsonReset();
	string name = "Data/Scene/SceneManager.json";
	json_["start"] = sceneName_;
	string s = json_.dump(4);
	ofstream writingFile;
	writingFile.open(name, std::ios::out);
	writingFile << s << std::endl;
	writingFile.close();
}


#endif


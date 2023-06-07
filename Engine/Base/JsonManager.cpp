#include "JsonManager.h"
#include <iostream>
#include <fstream>

using namespace IFE;
using namespace std;

void IFE::JsonManager::SetSceneName(std::string s)
{
	sceneName = s;
}

JsonManager* IFE::JsonManager::Instance()
{
	static JsonManager inst;
	return &inst;
}

float IFE::JsonManager::InputFloat(std::string jsonName)
{
	return _json[jsonName];
}

std::int32_t IFE::JsonManager::InputInt(std::string jsonName)
{
	return _json[jsonName];
}

bool IFE::JsonManager::InputBool(std::string jsonName)
{
	return _json[jsonName];
}

uint32_t IFE::JsonManager::InputUINT(std::string jsonName)
{
	return _json[jsonName];
}

std::string IFE::JsonManager::InputString(std::string jsonName)
{
	return _json[jsonName];
}

Float2 IFE::JsonManager::InputFloat2(std::string jsonName)
{
	return Float2(_json[jsonName][0], _json[jsonName][1]);
}

Float3 IFE::JsonManager::InputFloat3(std::string jsonName)
{
	return Float3(_json[jsonName][0], _json[jsonName][1], _json[jsonName][2]);
}

Float4 IFE::JsonManager::InputFloat4(std::string jsonName)
{
	return Float4(_json[jsonName][0], _json[jsonName][1], _json[jsonName][2], _json[jsonName][3]);
}

void IFE::JsonManager::Input(std::string filename)
{
	JsonReset();
	string name = "Data/Scene/" + sceneName + "/" + filename + ".json";
	ifstream readingFile;
	readingFile.open(name, std::ios::in);
	if (!readingFile) {
		readingFile.close();
		error = true;
		return;
	}
	readingFile >> _json;
	readingFile.close();
}

void IFE::JsonManager::JsonReset()
{
	nlohmann::json j;
	_json = j;
}

std::string IFE::JsonManager::SceneInit()
{
	JsonReset();
	string name = "Data/Scene/SceneManager.json";
	ifstream readingFile;
	readingFile.open(name, std::ios::in);
	if (!readingFile) {
		readingFile.close();
		error = true;
		return "";
	}
	readingFile >> _json;
	readingFile.close();
	return _json["start"];
}

nlohmann::json& IFE::JsonManager::GetJsonData()
{
	return _json;
}


#ifdef _DEBUG
void IFE::JsonManager::OutputFloat(std::string jsonName, float f)
{
	_json[jsonName] = f;
}

void IFE::JsonManager::OutputInt(std::string jsonName, int32_t i)
{
	_json[jsonName] = i;
}

void IFE::JsonManager::OutputBool(std::string jsonName, bool b)
{
	_json[jsonName] = b;
}

void IFE::JsonManager::OutputUINT(std::string jsonName, uint32_t i)
{
	_json[jsonName] = i;
}

void IFE::JsonManager::OutputString(std::string jsonName, std::string str)
{
	_json[jsonName] = str;
}

void IFE::JsonManager::OutputFloat2(std::string jsonName, Float2 f)
{
	_json[jsonName][0] = f.x;
	_json[jsonName][1] = f.y;
}

void IFE::JsonManager::OutputFloat3(std::string jsonName, Float3 f)
{
	_json[jsonName][0] = f.x;
	_json[jsonName][1] = f.y;
	_json[jsonName][2] = f.z;
}

void IFE::JsonManager::OutputFloat4(std::string jsonName, Float4 f)
{
	_json[jsonName][0] = f.x;
	_json[jsonName][1] = f.y;
	_json[jsonName][2] = f.z;
	_json[jsonName][3] = f.w;
}

void IFE::JsonManager::OutputFloat2(std::string jsonName, Vector2 v)
{
	_json[jsonName][0] = v.x;
	_json[jsonName][1] = v.y;
}

void IFE::JsonManager::OutputFloat3(std::string jsonName, Vector3 v)
{
	_json[jsonName][0] = v.x;
	_json[jsonName][1] = v.y;
	_json[jsonName][2] = v.z;
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
void IFE::JsonManager::OutputAndMakeDirectry(std::string filename, std::string directry)
{
	string dir = "Data/Scene/" + sceneName + "/" + directry;
	_mkdir(dir.c_str());
	string s = _json.dump(4);
	ofstream writingFile;
	string name = dir + "/" + filename + ".json";
	writingFile.open(name, std::ios::out);
	writingFile << s << std::endl;
	writingFile.close();
	JsonReset();
}

void IFE::JsonManager::MakeDirectry(std::string directry)
{
	string dir = "Data/Scene/" + sceneName + "/" + directry;
	_mkdir(dir.c_str());
}

void IFE::JsonManager::MakeSceneDirectry()
{
	string dir = "Data/Scene/" + sceneName;
	_mkdir(dir.c_str());
}

void IFE::JsonManager::SetInitScene()
{
	JsonReset();
	string name = "Data/Scene/SceneManager.json";
	_json["start"] = sceneName;
	string s = _json.dump(4);
	ofstream writingFile;
	writingFile.open(name, std::ios::out);
	writingFile << s << std::endl;
	writingFile.close();
}


#endif


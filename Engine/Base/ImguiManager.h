#pragma once
#include <string>
#include <vector>
#include <array>
#include <functional>
#include "IFMath.h"

#ifdef _DEBUG
namespace IFE
{
	class ComponentManager;
	class Component;
	class Object3D;
	struct Texture;
	class ImguiManager
	{
		static const int32_t NUM_FRAMES_IN_FLIGHT = 3;
		ImguiManager() {}
		ImguiManager(const ImguiManager&) {}
		ImguiManager& operator=(const ImguiManager&) {}
		~ImguiManager() {}
	public:
		static std::string openComponentName;
		static ImguiManager* Instance();
		void Initialize();
		void StartNewFrame();
		void Draw();
		void Demo();
		void EndGUI();
		void ObjectManagerGUI(bool* flagadd, bool* flagdelete, bool* prefab, bool* fmove);
		void ComponentGUI(const std::string& objectName, const std::function<void(void)>& ComponentFunc, const std::function<void(Component*)>& addFunc/*, const std::function<void(Component*)>& modelFunc*/);
		void ComponentGUI2D(const std::string& objectName, const std::function<void(void)>& ComponentFunc, const std::function<void(Component*)>& addFunc/*, const std::function<void(std::string)>& texFunc*/);
		bool ObjectGUI(const std::string& objectName, const  bool& flagdelete, const bool& move, bool* moveFlag, Object3D* obj);
		bool SpriteGUI(const std::string& spriteName, const bool& flagdelete, const bool& m, bool* moveFlag);
		bool EmitterGUI(const std::string& spriteName, const bool& flagdelete);
		bool ObjectAddGUI(std::string* newObjectName, std::string* modelName, const std::function<std::string(void)> &guiFunc);
		void NewGUI(const std::string& guiName, const int32_t& flag = 0);
		void ModelManagerGUI(bool* flagadd, bool* flagdelete);
		bool ModelAddGUI(std::string* newObjectName, std::string* newFilename, uint16_t* settings, bool* smooth);
		int32_t GetModelNameGUI(const std::vector<std::string>&names);
		void ModelGUI(const std::string& modelName);
		void ComponentGUI(const std::function<void(void)>& guiFunc, const std::function<void(void)>& deleteFunc, const std::string& componentName);
		void ChangeTextureGUI(const std::function<void(std::string)>& guiFunc);
		void TextureGUI(bool* load, bool* search);
		void TextureLoadGUI(const std::function<void(std::string)>&);
		void ShowTextureGUI(const std::array<Texture, 1024>& texture);
		void SearchTextureGUI(const std::array<Texture, 1024>& texture);


		void TextGUI(const std::string& text);
		void DragIntGUI(int32_t* i, const std::string& label, const float& speed = 1, const int32_t& min = 0, const int32_t& max = 0);
		void DragFloatGUI(float* f, const std::string& label, const float& speed = 0.25, const float& min = 0, const float& max = 0);
		void DragFloat2GUI(Float2* f, const std::string& label, const float& speed = 0.25, const float& min = 0, const float& max = 0);
		void DragFloat3GUI(Float3* f, const std::string& label, const float& speed = 0.25, const float& min = 0, const float& max = 0);
		void DragFloat4GUI(Float4* f, const std::string& label, const float& speed = 0.25, const float& min = 0, const float& max = 0);
		void ColorEdit4GUI(Float4* color, const std::string& label);
		void CheckBoxGUI(bool* flag, const std::string& label);
		void CollapsingHeaderGUI(const std::string& label, const std::function<void(void)>& guiFunc);
		bool ButtonGUI(const std::string& buttonName);
		void RadioButtonGUI(const std::string& buttonName, int32_t* changeNum, const int32_t& buttonNum);
		bool NewTreeNode(const std::string& label);
		void EndTreeNode();
		Object3D* SetObject(Object3D* obj, const std::string& label);
	};
}
#endif
#pragma once
#include <d3d12.h>
#include <vector>
#include <memory>
#include <string>
#include "ConstStruct.h"
#include "IFMath.h"
#include "IndexBuffer.h"
#include "Component.h"
#include "ModelStruct.h"
#include "AddModelSettings.h"

namespace IFE
{
	class FBXModel :public Component
	{
		using Component::Component;
		template<class T> using vector = std::vector<T>;
	private:
		AddModelSettings setting;

	public:
		std::vector<std::unique_ptr<Node>> nodes{};
		std::vector<Bone> bones{};
		std::vector<Animation> animations{};
		std::vector<Matrix> transformMatrixs{};
		std::string fileName;


	public:
		void Initialize()override;
		void Update()override {};
		void Draw()override;
		void BoneTransform(float TimeInSeconds);
		void SetSettings(AddModelSettings setting);
	private:
		void ReadNodeHeirarchy(float AnimationTime, Node* pNode);

	public:
#ifdef _DEBUG
		void DebugGUI()override;
		void OutputScene(std::string object3d);
#endif
		void LoadingScene(std::string object3d);
	};
};

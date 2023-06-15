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
		AddModelSettings setting_;

	public:
		std::vector<std::unique_ptr<Node>> nodes_{};
		std::vector<Bone> bones_{};
		std::vector<Animation> animations_{};
		std::vector<Matrix> transformMatrixs_{};
		std::string fileName_;


	public:
		void Initialize()override;
		void Update()override {};
		void Draw()override;
		void BoneTransform(float TimeInSeconds);
		void SetSettings(const AddModelSettings& setting);
	private:
		void ReadNodeHeirarchy(float AnimationTime, Node* pNode);

	public:
#ifdef _DEBUG
		void DebugGUI()override;
		void OutputScene(const std::string& object3d);
#endif
		void LoadingScene(const std::string& object3d);
	};
};

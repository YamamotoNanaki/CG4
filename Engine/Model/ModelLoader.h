#pragma once
#include "GraphicsAPI.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "ModelStruct.h"
#include <string>
#include <vector>
#include <memory>

struct aiScene;
struct aiNode;
struct aiMesh;
namespace IFE
{
	class FBXModel;
	class ModelLoader
	{
	private:
		std::vector<std::unique_ptr<Node>> nodes;
		std::vector<Bone> bones;
		std::vector<Animation> animations;
		std::string filename;
		std::vector<std::string>m_BoneMapping;
		bool objType = false;
	private:
		void ParseNodeRecursive(const aiScene* scene, aiNode* node, Node* targetParent = nullptr);
		Mesh* ProcessMesh(const aiScene* scene, aiMesh* mesh);
	public:
		FBXModel* FBXLoad(std::string fileName, std::string fileType = ".fbx", bool smooth = false);
	};
};

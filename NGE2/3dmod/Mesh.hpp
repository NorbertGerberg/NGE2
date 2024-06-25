/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MESH_HPP
#define MESH_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Shader.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshContainer.hpp"
#include <Renderer.hpp>
#include <vector>
#include <functional>

class eMesh
{
public:
									~eMesh();
	void							ShouldOptimize(const bool vl);
	bool							IsOptimized();
	std::vector<eMeshContainer>&	GetContainers();

	void							Load(eString fileName);
	void							LoadMem(eString& mem);

	static void						InitIDB(std::vector<eTransformation3D>& transformations, eIDB& idb);

	void							DrawContainer(uint id, eLayer* layer, eTransformation3D transformation, eMaterial* mat = nullptr);
	void							Draw(eLayer* layer, eTransformation3D transformation, eMaterial* mat = nullptr);
	const int						DrawInstances(eLayer* layer, std::vector<eTransformation3D>& transformations, eMaterial* mat = nullptr);
	void							DrawInstances(eLayer* layer, eIDB& idb, eMaterial* mat = nullptr);
	const int						DrawContainerInstances(uint id, eLayer* layer, std::vector<eTransformation3D>& transformations, eMaterial* mat = nullptr);
	void							DrawContainerInstances(uint id, eLayer* layer, eIDB& idb, eMaterial* mat = nullptr);

	void							SetDrawFnc(std::function<eShader*(eMeshContainer&, eLayer*, eTransformation3D, eMaterial*)> fnc);

	static void						SetRenderer(eRenderer* renderer);
	static eRenderer*				GetRenderer();

protected:
	std::vector<eMeshContainer>															mSubMeshes;
	bool																				mOptimize = true;
	std::function<eShader*(eMeshContainer&, eLayer*, eTransformation3D, eMaterial*)>	mDrawFnc;
	static eRenderer*																	mRenderer;

	void							ProcessNode(aiNode* node, const aiScene* scene);
	eMeshContainer					ProcessMesh(aiMesh* mesh, const aiScene* scene);
	eMeshContainer					CreateMesh(std::vector<eMeshVertex> vertices, std::vector<uint16> indices);
	bool							DrawSingleBegin(eMeshContainer& container, eLayer* layer, eTransformation3D transformation, eMaterial* mat);
	void							DrawSingleEnd(eShader* shader, eMeshContainer& container, eLayer* layer);

	static const int				DrawInsBegin(std::vector<eTransformation3D>& transformations, bgfx::InstanceDataBuffer& idb);
	void							DrawInsEnd(eMeshContainer& container, eTransformation3D& first, eLayer* layer, eMaterial* mat, bgfx::InstanceDataBuffer& idb);
};
#endif
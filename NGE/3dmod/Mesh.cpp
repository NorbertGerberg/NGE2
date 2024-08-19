/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Mesh.hpp"
#include <Console.hpp>
#include <Tools.hpp>
#include <meshoptimizer.h>

//! Public pointer to renderer
eRenderer* eMesh::mRenderer = nullptr;

eMesh::~eMesh()
{
	for (auto& it : mSubMeshes)
	{
		it.mVertices.clear();
		it.mIndices.clear();
		eGraphics::Destroy(it.mVbh);
		eGraphics::Destroy(it.mIbh);
	}
	mSubMeshes.clear();
}

//! Whether or not the mesh should be optimized using meshoptimizer
void eMesh::ShouldOptimize(const bool vl)
{
	bOptimize = vl;
}

//!
bool eMesh::IsOptimized()
{
	return bOptimize;
}

//!
std::vector<eMeshContainer>& eMesh::GetContainers()
{
	return mSubMeshes;
}

//!
void eMesh::Load(eString fileName)
{
	Assimp::Importer importer;
	uint flags = aiProcess_FlipUVs
		| aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_CalcTangentSpace
		| aiProcess_SplitLargeMeshes
		| aiProcess_OptimizeMeshes;

	const aiScene* scene = importer.ReadFile(fileName.Get(), flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		eConsole::PrintLog("eMesh::Load", "Failed to load model at '" + fileName.GetStrg() + "'!");
		eConsole::WriteToDisk();
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

//!
void eMesh::LoadMem(eString& mem)
{
	Assimp::Importer importer;
	uint flags = aiProcess_FlipUVs
		| aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_CalcTangentSpace
		| aiProcess_SplitLargeMeshes
		| aiProcess_OptimizeMeshes;

	const aiScene* scene = importer.ReadFileFromMemory(mem.GetStrg().data(), mem.Lenght(), flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		eConsole::PrintLog("eMesh::LoadMem", "Failed to load model from memory!");
		eConsole::WriteToDisk();
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

//!
void eMesh::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mSubMeshes.push_back(ProcessMesh(mesh, scene));
	}

	for (uint i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene);
}

//!
eMeshContainer eMesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<eMeshVertex>	vertices;
	std::vector<uint16>			indices;

	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		eMeshVertex vertex;

		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.nx = mesh->mNormals[i].x;
			vertex.ny = mesh->mNormals[i].y;
			vertex.nz = mesh->mNormals[i].z;
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.u = mesh->mTextureCoords[0][i].x;
			vertex.v = mesh->mTextureCoords[0][i].y;

			vertex.tx = mesh->mTangents[i].x;
			vertex.ty = mesh->mTangents[i].y;
			vertex.tz = mesh->mTangents[i].z;

			vertex.bx = mesh->mBitangents[i].x;
			vertex.by = mesh->mBitangents[i].y;
			vertex.bz = mesh->mBitangents[i].z;
		}
		else
		{
			vertex.u = 0.0f;
			vertex.v = 0.0f;
		}
		vertices.push_back(vertex);
	}

	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return CreateMesh(vertices, indices);
}

//!
eMeshContainer eMesh::CreateMesh(std::vector<eMeshVertex> vertices, std::vector<uint16> indices)
{
	//Meshoptimizer code
	std::vector<eMeshVertex>	_rt_vertices = vertices;
	std::vector<uint16>			_rt_indices = indices;

	if (bOptimize)
	{
		meshopt_optimizeVertexCache(&_rt_indices[0], &_rt_indices[0], _rt_indices.size(), _rt_vertices.size());
		meshopt_optimizeVertexFetch(&_rt_vertices[0], &_rt_indices[0], _rt_indices.size(), &_rt_vertices[0], _rt_vertices.size(), sizeof(eMeshVertex));
	}

	eMeshContainer rtVl;
	rtVl.mVertices = _rt_vertices;
	rtVl.mIndices = _rt_indices;

	eVertexLayout vlayout;
	vlayout.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
		.end();

	rtVl.mVbh = eShader::CreateVertexBuffer(&vertices[0], vertices.size() * sizeof(eMeshVertex), vlayout);
	if (!bgfx::isValid(rtVl.mVbh))
	{
		eConsole::PrintLog("eMesh::CreateMesh", "Failed to create vbh.");
		eConsole::WriteToDisk();
		return {};
	}

	rtVl.mIbh = eShader::CreateIndexBuffer(&indices[0], indices.size() * sizeof(uint16));
	if (!bgfx::isValid(rtVl.mIbh))
	{
		eConsole::PrintLog("eMesh::CreateMesh", "Failed to create ibh.");
		eConsole::WriteToDisk();
		return {};
	}
	return rtVl;
}

//!
void eMesh::InitIDB(std::vector<eTransformation3D>& transformations, eIDB& idb)
{
	idb.mCouldNotDraw = DrawInsBegin(transformations, idb.mBuffer);
}

//!
void eMesh::DrawContainer(uint id, eLayer* layer, eTransformation3D transformation, eMaterial* mat)
{
	eMeshContainer& container = mSubMeshes[id];
	if (!DrawSingleBegin(container, layer, transformation, mat))
		return;
	eShader* shader = mDrawFnc(container, layer, transformation, mat);
	DrawSingleEnd(shader, container, layer);
}

//!
void eMesh::Draw(eLayer* layer, eTransformation3D transformation, eMaterial* mat)
{
	for (auto& it : mSubMeshes)
	{
		if (!DrawSingleBegin(it, layer, transformation, mat))
			continue;
		eShader* shader = mDrawFnc(it, layer, transformation, mat);
		DrawSingleEnd(shader, it, layer);
	}
}

//!
const int eMesh::DrawInstances(eLayer* layer, std::vector<eTransformation3D>& transformations, eMaterial* mat)
{
	eViewport3D* viewport = layer->GetViewport();
	if (viewport == nullptr) return -1;

	bgfx::InstanceDataBuffer idb;
	const int rtVl = DrawInsBegin(transformations, idb);

	for (auto& it : mSubMeshes)
	{
		eMaterial* useMat = it.mMat;

		bool noMat = true;
		if (mat != nullptr)
		{
			noMat = false;
			useMat = mat;
		}
		else if (it.mMat != nullptr)
			noMat = false;

		if(!noMat)
			DrawInsEnd(it, transformations[0], layer, useMat, idb);
	}

	return rtVl;
}

//!
void eMesh::DrawInstances(eLayer* layer, eIDB& idb, eMaterial* mat)
{
	eViewport3D* viewport = layer->GetViewport();
	if (viewport == nullptr) return;

	eTransformation3D empt = {};

	for (auto& it : mSubMeshes)
	{
		eMaterial* useMat = it.mMat;

		bool noMat = true;
		if (mat != nullptr)
		{
			noMat = false;
			useMat = mat;
		}
		else if (it.mMat != nullptr)
			noMat = false;

		if (!noMat)
			DrawInsEnd(it, empt, layer, useMat, idb.mBuffer);
	}
}

//!
const int eMesh::DrawContainerInstances(uint id, eLayer* layer, std::vector<eTransformation3D>& transformations, eMaterial* mat)
{
	eViewport3D* viewport = layer->GetViewport();
	if (viewport == nullptr) return -1;

	bgfx::InstanceDataBuffer idb;
	const int rtVl = DrawInsBegin(transformations, idb);

	eMeshContainer& cont = mSubMeshes[id];

	eMaterial* useMat = cont.mMat;

	bool noMat = true;
	if (mat != nullptr)
	{
		noMat = false;
		useMat = mat;
	}
	else if (cont.mMat != nullptr)
		noMat = false;

	if (!noMat)
		DrawInsEnd(cont, transformations[0], layer, useMat, idb);

	return rtVl;
}

//!
void eMesh::DrawContainerInstances(uint id, eLayer* layer, eIDB& idb, eMaterial* mat)
{
	eViewport3D* viewport = layer->GetViewport();
	if (viewport == nullptr) return;

	eMeshContainer& cont = mSubMeshes[id];

	eMaterial* useMat = cont.mMat;

	eTransformation3D empt = {};

	bool noMat = true;
	if (mat != nullptr)
	{
		noMat = false;
		useMat = mat;
	}
	else if (cont.mMat != nullptr)
		noMat = false;

	if (!noMat)
		DrawInsEnd(cont, empt, layer, useMat, idb.mBuffer);
}

//!
const int eMesh::DrawInsBegin(std::vector<eTransformation3D>& transformations, bgfx::InstanceDataBuffer& idb)
{
	const int insCnt = transformations.size();
	const uint16 insStride = 64;
	uint32 drawnIns = bgfx::getAvailInstanceDataBuffer(insCnt, insStride);

	const int couldNotDraw = insCnt - drawnIns;

	bgfx::allocInstanceDataBuffer(&idb, drawnIns, insStride);

	uint8* data = idb.data;

	for (uint32 i = 0; i < drawnIns; i++)
	{
		const mat4 mtx = eTools::Transform(transformations[i]);
		const float* _mtx = eMath::value_ptr(mtx);
		memcpy(data, _mtx, insStride);
		data += insStride;
	}

	return couldNotDraw;
}

//!
void eMesh::DrawInsEnd(eMeshContainer& container, eTransformation3D& first, eLayer* layer, eMaterial* mat, bgfx::InstanceDataBuffer& idb)
{
	eShader* shader = mDrawFnc(container, layer, first, mat);

	shader->ApplyVertexBuffer(0, container.mVbh);
	shader->ApplyIndexBuffer(container.mIbh);

	bgfx::setInstanceDataBuffer(&idb);

	eMaterialPrp* matPrp = mat->GetProperties();
	if (matPrp->bTwoSides)
	{
		eGraphics::SetState(BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_MSAA);
	}
	else
		eGraphics::SetState();

	shader->Submit(layer->GetProperties().mViewId, false);
}

//!
void eMesh::SetDrawFnc(std::function<eShader*(eMeshContainer&, eLayer*, eTransformation3D, eMaterial*)> fnc)
{
	mDrawFnc = fnc;
}

//!
bool eMesh::DrawSingleBegin(eMeshContainer& container, eLayer* layer, eTransformation3D transformation, eMaterial* mat)
{
	eViewport3D* viewport = layer->GetViewport();
	if (viewport == nullptr)
		return false;

	eMaterial* useMat = container.mMat;

	bool noMat = true;
	if (mat != nullptr)
	{
		noMat = false;
		useMat = mat;
	}
	else if (container.mMat != nullptr)
		noMat = false;

	eMaterialPrp* matPrp = useMat->GetProperties();

	if (matPrp->bTwoSides)
	{
		eGraphics::SetState(BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_MSAA);
	}
	else
		eGraphics::SetState();

	mat4 mdl = eTools::Transform(transformation);
	bgfx::setTransform(eMath::value_ptr(mdl));

	return !noMat;
}

//!
void eMesh::DrawSingleEnd(eShader* shader, eMeshContainer& container, eLayer* layer)
{
	shader->ApplyVertexBuffer(0, container.mVbh);
	shader->ApplyIndexBuffer(container.mIbh);
	shader->Submit(layer->GetProperties().mViewId, false);
}

void eMesh::SetRenderer(eRenderer* renderer)
{
	mRenderer = renderer;
}

//!
eRenderer* eMesh::GetRenderer()
{
	return mRenderer;
}
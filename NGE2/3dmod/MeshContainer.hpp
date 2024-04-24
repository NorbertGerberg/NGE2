/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MESHCONTAINER_HPP
#define MESHCONTAINER_HPP
#include <Types.hpp>
#include <Graphics.hpp>
#include "MeshVertex.hpp"
#include "Material.hpp"
#include <vector>

struct eMeshContainer
{
	std::vector<eMeshVertex>	mVertices;
	std::vector<uint16>			mIndices;
	eVertexBufferHandle			mVbh;
	eIndexBufferHandle			mIbh;
	eMaterial*					mMat;
};
#endif
/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MESHVERTEX_HPP
#define MESHVERTEX_HPP
#include <Types.hpp>

struct eMeshVertex
{
	//Position
	real3 x;
	real3 y;
	real3 z;

	//Normal
	real3 nx;
	real3 ny;
	real3 nz;

	//TexCoord
	real3 u;
	real3 v;

	//Tangent
	real3 tx;
	real3 ty;
	real3 tz;

	//Bitangent
	real3 bx;
	real3 by;
	real3 bz;
};
#endif
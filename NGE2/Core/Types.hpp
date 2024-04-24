/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef TYPES_HPP
#define TYPES_HPP
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#define eMath glm

typedef double												real6;
typedef float												real3;
typedef long double											decimal;

typedef __int8												int8;
typedef __int16												int16;
typedef __int32												int32;
typedef __int64												int64;
typedef unsigned int										uint;
typedef unsigned __int8										uint8;
typedef unsigned __int16									uint16;
typedef unsigned __int32									uint32;
typedef unsigned __int64									uint64;

typedef std::string											strg;
typedef unsigned long										ulong;
typedef unsigned short										ushort;
typedef signed short										sshort;

typedef glm::vec<4, real3, glm::qualifier::defaultp>		vec4;
typedef glm::vec<3, real3, glm::qualifier::defaultp>		vec3;
typedef glm::vec<2, real3, glm::qualifier::defaultp>		vec2;
typedef glm::mat<4, 4, real3, glm::qualifier::defaultp>		mat4;
typedef glm::mat<3, 3, real3, glm::qualifier::defaultp>		mat3;
typedef glm::mat<2, 2, real3, glm::qualifier::defaultp>		mat2;
typedef glm::qua<real3, glm::qualifier::defaultp>			quat;

typedef glm::vec<4, int, glm::qualifier::defaultp>			vec4i;
typedef glm::vec<3, int, glm::qualifier::defaultp>			vec3i;
typedef glm::vec<2, int, glm::qualifier::defaultp>			vec2i;

typedef glm::vec<4, real6, glm::qualifier::defaultp>		vec4d;
typedef glm::vec<3, real6, glm::qualifier::defaultp>		vec3d;
typedef glm::vec<2, real6, glm::qualifier::defaultp>		vec2d;
typedef glm::mat<4, 4, real6, glm::qualifier::defaultp>		mat4d;
typedef glm::mat<3, 3, real6, glm::qualifier::defaultp>		mat3d;
typedef glm::mat<2, 2, real6, glm::qualifier::defaultp>		mat2d;
typedef glm::qua<real6, glm::qualifier::defaultp>			quatd;
#endif
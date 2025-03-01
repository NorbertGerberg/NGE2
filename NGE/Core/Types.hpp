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

//! \defgroup Core
//! \brief Core of engine - Must be included with all other libraries
//! \defgroup Engine
//! \brief Base engine features like input, rendering, etc.
//! \defgroup World
//! \brief Entity/Component engine
//! \defgroup Content
//! \brief Asset management using packages
//! \defgroup Tdmod
//! \brief 3dmod - for 3D rendering

#define eMath glm

typedef double												real6;
typedef float												real3;
typedef long double											decimal;

typedef signed char											int8;
typedef short												int16;
typedef int													int32;
typedef signed long long									int64;
typedef unsigned int										uint;
typedef unsigned char										uint8;
typedef unsigned unsigned short								uint16;
typedef unsigned int										uint32;
typedef unsigned unsigned long long							uint64;

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
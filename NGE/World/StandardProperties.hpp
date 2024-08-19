/*=========================================
	Copyright 2024-2025 Norbert Gerberg

	Standard properties
=========================================*/
#ifndef STANDARDPROPERTIES_HPP
#define STANDARDPROPERTIES_HPP
#include <Types.hpp>
#include <String.hpp>

//! Standard properties
//! \ingroup World
namespace eStandardProperties
{
	eString stp_int(void* vl);
	void	stp_int_(void* ptr, eString vl);

	eString stp_int8(void* vl);
	void	stp_int8_(void* ptr, eString vl);

	eString stp_int16(void* vl);
	void	stp_int16_(void* ptr, eString vl);

	eString stp_int32(void* vl);
	void	stp_int32_(void* ptr, eString vl);

	eString stp_int64(void* vl);
	void	stp_int64_(void* ptr, eString vl);

	eString stp_uint(void* vl);
	void	stp_uint_(void* ptr, eString vl);

	eString stp_uint8(void* vl);
	void	stp_uint8_(void* ptr, eString vl);

	eString stp_uint16(void* vl);
	void	stp_uint16_(void* ptr, eString vl);

	eString stp_uint32(void* vl);
	void	stp_uint32_(void* ptr, eString vl);

	eString stp_uint64(void* vl);
	void	stp_uint64_(void* ptr, eString vl);

	eString stp_String(void* vl);
	void	stp_String_(void* ptr, eString vl);

	eString stp_Real3(void* vl);
	void	stp_Real3_(void* ptr, eString vl);

	eString stp_Real6(void* vl);
	void	stp_Real6_(void* ptr, eString vl);

	eString stp_Bool(void* vl);
	void	stp_Bool_(void* ptr, eString vl);

	eString stp_Vec2(void* vl);
	void	stp_Vec2_(void* ptr, eString vl);

	eString stp_Vec3(void* vl);
	void	stp_Vec3_(void* ptr, eString vl);

	eString stp_Vec4(void* vl);
	void	stp_Vec4_(void* ptr, eString vl);
}
#endif
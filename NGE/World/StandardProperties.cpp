/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "StandardProperties.hpp"
#include <sstream>

eString eStandardProperties::stp_int(void* vl)
{
	return eString::ToString(*reinterpret_cast<int*>(vl));
}

void eStandardProperties::stp_int_(void* ptr, eString vl)
{
	int* _ptr = reinterpret_cast<int*>(ptr);
	*_ptr = eString::ToInt<int>(vl);
}

eString eStandardProperties::stp_int8(void* vl)
{
	return eString::ToString(*reinterpret_cast<int8*>(vl));
}

void eStandardProperties::stp_int8_(void* ptr, eString vl)
{
	int8* _ptr = reinterpret_cast<int8*>(ptr);
	*_ptr = eString::ToInt<int8>(vl);
}

eString eStandardProperties::stp_int16(void* vl)
{
	return eString::ToString(*reinterpret_cast<int16*>(vl));
}

void eStandardProperties::stp_int16_(void* ptr, eString vl)
{
	int16* _ptr = reinterpret_cast<int16*>(ptr);
	*_ptr = eString::ToInt<int16>(vl);
}

eString eStandardProperties::stp_int32(void* vl)
{
	return eString::ToString(*reinterpret_cast<int32*>(vl));
}

void eStandardProperties::stp_int32_(void* ptr, eString vl)
{
	int32* _ptr = reinterpret_cast<int32*>(ptr);
	*_ptr = eString::ToInt<int32>(vl);
}

eString eStandardProperties::stp_int64(void* vl)
{
	return eString::ToString(*reinterpret_cast<int64*>(vl));
}

void eStandardProperties::stp_int64_(void* ptr, eString vl)
{
	int64* _ptr = reinterpret_cast<int64*>(ptr);
	*_ptr = eString::ToInt<int64>(vl);
}

eString eStandardProperties::stp_uint(void* vl)
{
	return eString::ToString(*reinterpret_cast<uint*>(vl));
}

void eStandardProperties::stp_uint_(void* ptr, eString vl)
{
	uint* _ptr = reinterpret_cast<uint*>(ptr);
	*_ptr = eString::ToInt<uint>(vl);
}

eString eStandardProperties::stp_uint8(void* vl)
{
	return eString::ToString(*reinterpret_cast<uint8*>(vl));
}

void eStandardProperties::stp_uint8_(void* ptr, eString vl)
{
	uint8* _ptr = reinterpret_cast<uint8*>(ptr);
	*_ptr = eString::ToInt<uint8>(vl);
}

eString eStandardProperties::stp_uint16(void* vl)
{
	return eString::ToString(*reinterpret_cast<uint16*>(vl));
}

void eStandardProperties::stp_uint16_(void* ptr, eString vl)
{
	uint16* _ptr = reinterpret_cast<uint16*>(ptr);
	*_ptr = eString::ToInt<uint16>(vl);
}

eString eStandardProperties::stp_uint32(void* vl)
{
	return eString::ToString(*reinterpret_cast<uint32*>(vl));
}

void eStandardProperties::stp_uint32_(void* ptr, eString vl)
{
	uint32* _ptr = reinterpret_cast<uint32*>(ptr);
	*_ptr = eString::ToInt<uint32>(vl);
}

eString eStandardProperties::stp_uint64(void* vl)
{
	return eString::ToString(*reinterpret_cast<uint64*>(vl));
}

void eStandardProperties::stp_uint64_(void* ptr, eString vl)
{
	uint64* _ptr = reinterpret_cast<uint64*>(ptr);
	*_ptr = eString::ToInt<uint64>(vl);
}

eString eStandardProperties::stp_String(void* vl)
{
	eString rtVl = *reinterpret_cast<eString*>(vl);
	rtVl.Replace(' ', '|');
	return rtVl;
}

void eStandardProperties::stp_String_(void* ptr, eString vl)
{
	eString* _ptr = reinterpret_cast<eString*>(ptr);
	eString _vl(vl);
	_vl.Replace('|', ' ');
	*_ptr = _vl;
}

eString eStandardProperties::stp_Real3(void* vl)
{
	return eString::ToString(*reinterpret_cast<real3*>(vl));
}

void eStandardProperties::stp_Real3_(void* ptr, eString vl)
{
	real3* _ptr = reinterpret_cast<real3*>(ptr);
	*_ptr = eString::ToReal3(vl);
}

eString eStandardProperties::stp_Real6(void* vl)
{
	return eString::ToString(*reinterpret_cast<real6*>(vl));
}

void eStandardProperties::stp_Real6_(void* ptr, eString vl)
{
	real6* _ptr = reinterpret_cast<real6*>(ptr);
	*_ptr = eString::ToReal6(vl);
}

eString eStandardProperties::stp_Bool(void* vl)
{
	return eString::ToString(*reinterpret_cast<bool*>(vl));
}

void eStandardProperties::stp_Bool_(void* ptr, eString vl)
{
	bool* _ptr = reinterpret_cast<bool*>(ptr);
	*_ptr = eString::ToInt<int>(vl);
}

eString eStandardProperties::stp_Vec2(void* vl)
{
	vec2 _vl = *reinterpret_cast<vec2*>(vl);
	eString rtVl = eString::ToString(_vl.x);
	rtVl += eTEXT(";") + eString::ToString(_vl.y);
	return rtVl;
}

void eStandardProperties::stp_Vec2_(void* ptr, eString vl)
{
	vec2* _ptr = reinterpret_cast<vec2*>(ptr);
	eString _x, _y;
	std::stringstream ss(vl.GetStrg());
	std::getline(ss, _x.Ref(), ';');
	std::getline(ss, _y.Ref(), ';');
	*_ptr = vec2(eString::ToReal3(_x), eString::ToReal3(_y));
}

eString eStandardProperties::stp_Vec3(void* vl)
{
	vec3 _vl = *reinterpret_cast<vec3*>(vl);
	eString rtVl = eString::ToString(_vl.x);
	rtVl += eTEXT(";") + eString::ToString(_vl.y);
	rtVl += eTEXT(";") + eString::ToString(_vl.z);
	return rtVl;
}

void eStandardProperties::stp_Vec3_(void* ptr, eString vl)
{
	vec3* _ptr = reinterpret_cast<vec3*>(ptr);
	eString _x, _y, _z;
	std::stringstream ss(vl.GetStrg());
	std::getline(ss, _x.Ref(), ';');
	std::getline(ss, _y.Ref(), ';');
	std::getline(ss, _z.Ref(), ';');
	*_ptr = vec3(eString::ToReal3(_x), eString::ToReal3(_y), eString::ToReal3(_z));
}

eString eStandardProperties::stp_Vec4(void* vl)
{
	vec4 _vl = *reinterpret_cast<vec4*>(vl);
	eString rtVl = eString::ToString(_vl.x);
	rtVl += eTEXT(";") + eString::ToString(_vl.y);
	rtVl += eTEXT(";") + eString::ToString(_vl.z);
	rtVl += eTEXT(";") + eString::ToString(_vl.w);
	return rtVl;
}

void eStandardProperties::stp_Vec4_(void* ptr, eString vl)
{
	vec4* _ptr = reinterpret_cast<vec4*>(ptr);
	eString _x, _y, _z, _w;
	std::stringstream ss(vl.GetStrg());
	std::getline(ss, _x.Ref(), ';');
	std::getline(ss, _y.Ref(), ';');
	std::getline(ss, _z.Ref(), ';');
	std::getline(ss, _w.Ref(), ';');
	*_ptr = vec4(eString::ToReal3(_x), eString::ToReal3(_y), eString::ToReal3(_z), eString::ToReal3(_w));
}
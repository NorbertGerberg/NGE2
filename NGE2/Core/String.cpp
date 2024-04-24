/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "String.hpp"
#include <algorithm>

eString::eString()
{
	mValue = "";
}

eString::eString(const eString& vl)
{
	mValue.clear();
	mValue = vl.mValue;
}

eString::eString(const char* vl)
{
	mValue = strg(vl);
}

eString::eString(strg vl)
{
	mValue = vl;
}

eString::~eString()
{
	mValue.clear();
}

void eString::Set(const char* vl)
{
	mValue.clear();
	mValue = strg(vl);
}

void eString::Add(const char* vl)
{
	mValue += strg(vl);
}

void eString::AddLine(const char* vl)
{
	mValue += "\n" + strg(vl);
}

void eString::Clear()
{
	mValue.clear();
}

const char* eString::Get()
{
	return mValue.c_str();
}

const strg& eString::GetStrg() const
{
	return mValue;
}

bool eString::Compare(eString s1)
{
	return (mValue == s1.GetStrg());
}

bool eString::Contains(eString s0)
{
	strg c0(s0.Get());
	return (mValue.find(c0) != strg::npos);
}

uint64 eString::Size()
{
	return mValue.size();
}

uint64 eString::Lenght()
{
	return mValue.length();
}

void eString::Set(eString vl)
{
	mValue.clear();
	mValue = strg(vl.Get());
}

void eString::Add(eString vl)
{
	mValue += strg(vl.Get());
}

void eString::AddLine(eString vl)
{
	mValue += "\n" + strg(vl.Get());
}

const bool& eString::IsEmpty() const
{
	return mValue.empty();
}

void eString::Replace(char th, char withMe)
{
	std::replace_if(mValue.begin(), mValue.end(), [th](char c) { return c == th; }, withMe);
}
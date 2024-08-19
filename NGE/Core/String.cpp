/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "String.hpp"
#include <algorithm>
#include <sstream>

eString::eString()
{
	mValue = "";
}

//! String constructor
eString::eString(const eString& vl)
{
	mValue.clear();
	mValue = vl.mValue;
}

//! String constructor with c-string
eString::eString(const char* vl)
{
	mValue = strg(vl);
}

//! String constructor with std::string
eString::eString(strg vl)
{
	mValue = vl;
}

//! String deconstructor
eString::~eString()
{
	mValue.clear();
}

//! Set text
void eString::Set(const char* vl)
{
	mValue.clear();
	mValue = strg(vl);
}

//! Add to text
void eString::Add(const char* vl)
{
	mValue += strg(vl);
}

//! Add to text in new line
void eString::AddLine(const char* vl)
{
	mValue += "\n" + strg(vl);
}

//! Clear text
void eString::Clear()
{
	mValue.clear();
}

//! Return text as c-string
const char* eString::Get()
{
	return mValue.c_str();
}

//! Return text as std::string
const strg& eString::GetStrg() const
{
	return mValue;
}

//! Compare to string
bool eString::Compare(eString s1)
{
	return (mValue == s1.GetStrg());
}

//! Token check
bool eString::Contains(eString s0)
{
	strg c0(s0.Get());
	return (mValue.find(c0) != strg::npos);
}

//! Returns text size
uint64 eString::Size()
{
	return mValue.size();
}

//! Returns text lenght
uint64 eString::Lenght()
{
	return mValue.length();
}

//! Set text by string
void eString::Set(eString vl)
{
	mValue.clear();
	mValue = strg(vl.Get());
}

//! Add to text by string
void eString::Add(eString vl)
{
	mValue += strg(vl.Get());
}

//! Add to text by string in new line
void eString::AddLine(eString vl)
{
	mValue += "\n" + strg(vl.Get());
}

//! Returns true if text is empty
const bool& eString::IsEmpty() const
{
	return mValue.empty();
}

//! Returns true if token was found
const bool eString::Find(eString vl)
{
	return (mValue.find(vl.GetStrg()) != strg::npos);
}

//! Replaces a character with another one
void eString::Replace(char th, char withMe)
{
	std::replace_if(mValue.begin(), mValue.end(), [th](char c) { return c == th; }, withMe);
}

//! Replaces a std::string with an std::string
void eString::Replace(strg th, strg withMe)
{
	if (th.empty()) return;
	strg rtVl = "";
	std::istringstream iss(mValue);
	strg line;
	while (std::getline(iss, line, '\n'))
	{
		std::istringstream iss2(line);
		strg token;
		while (iss2 >> token)
		{
			if (token == th) rtVl += withMe;
			else rtVl += token;
			rtVl += " ";
		}

		rtVl += "\n";
	}
	mValue.clear();
	mValue = rtVl;
}

//! Replaces a string with a string
void eString::Replace(eString th, eString withMe)
{
	Replace(th.GetStrg(), withMe.GetStrg());
}

//! Reference to cpp string
strg& eString::Ref()
{
	return mValue;
}
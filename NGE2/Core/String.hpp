/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef STRING_HPP
#define STRING_HPP
#include "Types.hpp"

class eString
{
public:
	eString();
	eString(const eString& vl);
	eString(const char* vl);
	eString(strg vl);
	~eString();

	void Set(const char* vl);
	void Add(const char* vl);
	void AddLine(const char* vl);
	void Clear();
	const char* Get();
	const strg& GetStrg() const;

	void Set(eString vl);
	void Add(eString vl);
	void AddLine(eString vl);

	bool Compare(eString s1);
	bool Contains(eString s0);
	uint64 Size();
	uint64 Lenght();
	const bool& IsEmpty() const;

	void Replace(char th, char withMe);

	template<typename T>
	static eString ToString(T vl)
	{
		return eString(std::to_string(vl));
	}

	eString operator +(const char* in)
	{
		eString rtVl = eString(this->Get());
		rtVl.Add(in);
		return rtVl;
	}

	void operator +=(const char* in)
	{
		Add(in);
	}

	eString operator +(eString in)
	{
		eString rtVl = eString(this->Get());
		rtVl.Add(in);
		return rtVl;
	}

	void operator +=(eString in)
	{
		Add(in);
	}

	bool operator ==(eString in)
	{
		return (mValue == in.mValue);
	}

private:
	strg mValue;
};
#endif
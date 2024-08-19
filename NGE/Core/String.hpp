/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef STRING_HPP
#define STRING_HPP
#include "Types.hpp"
#include "Base.hpp"

#define TEXT(I) eString(I)

//! Simple string class using standard library
//! \ingroup Core
class eString : public eBase
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

	const bool Find(eString vl);

	void Replace(char th, char withMe);
	void Replace(strg th, strg withMe);
	void Replace(eString th, eString withMe);

	//!
	template<typename T>
	static eString ToString(T vl)
	{
		return eString(std::to_string(vl));
	}

	//!
	template<typename T>
	static T ToInt(eString& vl)
	{
		return ::atoi(vl.Get());
	}

	//!
	static real6 ToReal6(eString& vl)
	{
		return ::atof(vl.Get());
	}

	//!
	static real3 ToReal3(eString& vl)
	{
		return static_cast<real3>(::atof(vl.Get()));
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

	void operator +=(char in)
	{
		mValue += in;
	}

	void operator =(char in)
	{
		mValue = in;
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

	char operator [](uint in)
	{
		return mValue[in];
	}

	strg& Ref();

private:
	strg mValue;
};
#endif
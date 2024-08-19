/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef CONSOLE_HPP
#define CONSOLE_HPP
#include "Types.hpp"
#include "String.hpp"
#include "Base.hpp"
 
//! Console will output any issues or infos during runtime
//! \ingroup Core
class eConsole : public eBase
{
public:
	static void Print(eString msg);
	static void PrintLine(eString msg);
	static void PrintLog(eString head, eString msg);
	static void Clear();
	static eString Get();
	static void WriteToDisk();

private:
	static eString mLog;
};
#endif
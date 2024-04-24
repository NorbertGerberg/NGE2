/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef CONSOLE_HPP
#define CONSOLE_HPP
#include "Types.hpp"
#include "String.hpp"

class eConsole
{
public:
	static void Print(eString msg);
	static void PrintLine(eString msg);
	static void PrintLog(eString head, eString msg);
	static void Clear();
	static eString Get();
	static void WriteToDisk();

	static eString mCmdArg;

private:
	static eString mLog;
};
#endif
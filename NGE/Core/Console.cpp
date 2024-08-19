/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Console.hpp"
#include "EDR.hpp"

eString eConsole::mLog;

//! Print the string
void eConsole::Print(eString msg)
{
	mLog += msg;
}

//! Print the string in a new line
void eConsole::PrintLine(eString msg)
{
	mLog += eString("\n") + msg;
}

//! Print a log
void eConsole::PrintLog(eString head, eString msg)
{
	mLog += eString("\n") + head + eString(": ") + msg;
}

//! Clear the console
void eConsole::Clear()
{
	mLog.Clear();
}

//! Returns the console text
eString eConsole::Get()
{
	return mLog;
}

//! Write the console text to a crash file called 'CRASH.log'
void eConsole::WriteToDisk()
{
	EDR::WriteFile("CRASH.log", strg(mLog.Get()));
}
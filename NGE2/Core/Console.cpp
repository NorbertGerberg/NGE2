/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Console.hpp"
#include "EDR.hpp"

eString eConsole::mLog;
eString eConsole::mCmdArg;

void eConsole::Print(eString msg)
{
	mLog += msg;
}

void eConsole::PrintLine(eString msg)
{
	mLog += eString("\n") + msg;
}

void eConsole::PrintLog(eString head, eString msg)
{
	mLog += eString("\n") + head + eString(": ") + msg;
}

void eConsole::Clear()
{
	mLog.Clear();
}

eString eConsole::Get()
{
	return mLog;
}

void eConsole::WriteToDisk()
{
	EDR::WriteFile("CRASH.log", strg(mLog.Get()));
}
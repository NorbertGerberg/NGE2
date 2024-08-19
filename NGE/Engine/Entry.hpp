/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef ENTRY_HPP
#define ENTRY_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>
#include <Console.hpp>
#include "Window.hpp"
#include "Task.hpp"

//! Helper class for tasks
//! \ingroup Engine
template<typename T>
class eEntry : public eBase
{
public:
	~eEntry()
	{
		Release();
	}

	bool Initialize()
	{
		//	Hide CMD Window
		::ShowWindow(::GetConsoleWindow(), false);
#if _DEBUG
		//	Look for memory leaks
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

		if (!eWindow::INIT()) return false;
		mTask = new T();
		if (!mTask->Initialize())
		{
			delete mTask;
			eConsole::PrintLog("ERROR::eEntry::Initialize", "mTask->Initialize() returned false!");
			eConsole::WriteToDisk();
			return false;
		}
		eConsole::PrintLog("INFO::eEntry::Initialize", "Successfully initialized task!");
		return true;
	}

	void Run()
	{
		while (!mTask->bQuit)
		{
			eWindow::UPDATE();
			mTask->Update();
			mTask->Render();
		}
	}

	void Release()
	{
		if (mTask != nullptr)
		{
			mTask->Unload();
			delete mTask;
			mTask = nullptr;
		}
		eConsole::mCmdArg.Clear();
		eWindow::RELEASE();
	}

private:
	T* mTask;
};
#endif
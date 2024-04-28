/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include <Types.hpp>
#include <String.hpp>
#include <Console.hpp>
#include <Window.hpp>
#include "GameTask.hpp"

int main()
{
	//	Hide CMD Window
	::ShowWindow(::GetConsoleWindow(), false);
#if _DEBUG
	//	Look for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	eWindow::INIT();

	//	Create Game
	GameTask* game = new GameTask();
	if (!game->Initialize())
	{
		delete game;
		eConsole::PrintLog("ERROR::main", "game->Initialize() returned false!");
		eConsole::WriteToDisk();
		return 0;
	}

	while (!game->mQuit)
	{
		eWindow::UPDATE();
		game->Update();
		game->Render();
	}

	game->Unload();
	delete game;

	eConsole::mCmdArg.Clear();
	eWindow::RELEASE();
	return 0;
}
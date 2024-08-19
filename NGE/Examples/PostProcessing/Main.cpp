/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include <Entry.hpp>
#include "GameTask.hpp"

int main()
{
	eEntry<GameTask> entry;
	if (!entry.Initialize()) return 0;
	entry.Run();
	return 0;
}
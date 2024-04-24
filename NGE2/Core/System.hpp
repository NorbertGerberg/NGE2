/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include "Types.hpp"
#include <Wbemidl.h>

namespace eSystem
{
	//CPU
	const uint GetProcessorCount();

	//RAM
	const uint64 GetRAMsize();
	const uint64 GetAvailableRAMsize();
	const uint64 GetVirtualRAMsize();
	const uint64 GetAvailableVirtualRAMsize();

	//Drive
	const uint64 GetSpaceOnDisk();
	const uint64 GetAvailableSpaceOnDisk();
};
#endif
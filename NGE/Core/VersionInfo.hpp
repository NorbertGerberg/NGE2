/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef VERSIONINFO_HPP
#define VERSIONINFO_HPP
#define NGE_ENGINE_INFO_VERSION (uint16)200
#if defined(__clang__)
#define NGE_COMPILER_NAME "Clang"
#define NGE_COMPILER_VERSION __clang_version__
#elif defined(__GNUC__) || defined(__GNUG__)
#define NGE_COMPILER_NAME "GCC"
#define NGE_COMPILER_VERSION __VERSION__
#elif defined(_MSC_VER)
#define NGE_COMPILER_NAME "MSVC"
#define NGE_COMPILER_VERSION _MSC_VER
#elif defined(__INTEL_COMPILER)
#define NGE_COMPILER_NAME "Intel"
#define NGE_COMPILER_VERSION __INTEL_COMPILER
#else
#define NGE_COMPILER_NAME "Unknown"
#define NGE_COMPILER_VERSION 0
#endif
#endif
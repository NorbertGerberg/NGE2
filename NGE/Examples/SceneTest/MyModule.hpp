/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MYMODULE_HPP
#define MYMODULE_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Module.hpp>

class MyModule : public eModule
{
public:
	MyModule();
	bool Initialize() override;
	void Render() override;

private:
	eString mMsg;
};
#endif
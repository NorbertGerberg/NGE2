/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MYDOER_HPP
#define MYDOER_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Doer.hpp>

class MyDoer : public eDoer
{
public:
	MyDoer();
	bool Initialize() override;

private:
	void DestroyEvent();
};
#endif
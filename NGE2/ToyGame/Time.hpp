/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef TIME_HPP
#define TIME_HPP
#include <Types.hpp>

class eTime
{
public:
	real6 DeltaTime() const&;
	void Update();
	void SetSpeed(real3* vl);

private:
	real6 deltaTime = .0;
	real6 lastFrame = .0;
	real3* speed = nullptr;
};
#endif
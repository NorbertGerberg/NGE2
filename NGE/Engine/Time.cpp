/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Time.hpp"
#include <GLFW/glfw3.h>

real6 eTime::DeltaTime() const&
{
	return deltaTime;
}

void eTime::Update()
{
	real6 currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	deltaTime *= static_cast<real6>(*speed);
	return;
}

void eTime::SetSpeed(real3* vl)
{
	speed = vl;
}
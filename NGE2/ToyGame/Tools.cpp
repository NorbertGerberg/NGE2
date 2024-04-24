/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Tools.hpp"
#include <Console.hpp>
#include <bx/bx.h>
#include <bx/math.h>

const real3 eTools::CalcDistance(vec3 d1, vec3 d2)
{
	return eMath::length(d1 - d2);
}

mat4 eTools::Transform(eTransformation3D transformation)
{
	mat4 mdl = eMath::translate(mat4(1.0f), transformation.mPosition);
	mdl = eMath::rotate(mdl, eMath::radians(transformation.mRotation.x), vec3(1.0f, 0.0f, 0.0f));
	mdl = eMath::rotate(mdl, eMath::radians(transformation.mRotation.y), vec3(0.0f, 1.0f, 0.0f));
	mdl = eMath::rotate(mdl, eMath::radians(transformation.mRotation.z), vec3(0.0f, 0.0f, 1.0f));
	mdl = eMath::scale(mdl, transformation.mScale);
	return mdl;
}

eTransformation3D eTools::Decompose(mat4 mat)
{
	vec3 position;
	vec3 scale;
	quat rotation;
	vec3 dcSkew;
	vec4 dcPerspective;
	eMath::decompose(mat, scale, rotation, position, dcSkew, dcPerspective);
	rotation = eMath::conjugate(rotation);
	vec3 dcEulaRot = -eMath::degrees(eMath::eulerAngles(rotation));

	eTransformation3D rtVl;
	rtVl.mPosition = position;
	rtVl.mRotation = dcEulaRot;
	rtVl.mScale = scale;
	return rtVl;
}

bgfx::TextureHandle eTools::Framebuffer2Texture(bgfx::TextureHandle fb, vec2i size, bgfx::TextureFormat::Enum format, uint64 flags)
{
	bgfx::TextureHandle readBackTexture = bgfx::createTexture2D(
		(uint16)size.x
		, (uint16)size.y
		, false
		, 1
		, format
		, flags | BGFX_TEXTURE_BLIT_DST
	);

	bgfx::blit(
		0,
		readBackTexture,
		0, 0,
		fb,
		0, 0
	);

	return readBackTexture;
}

vec3 eTools::Get3DRaycastedMousePosition(vec2 mousePos, const real3 screenWidth, const real3 screenHeight,
	vec3 cameraPos, vec3 cameraUp, vec3 cameraFront, real3 cameraZoomVl, const real3 distance)
{
	real6 mouseX = mousePos.x;
	real6 mouseY = mousePos.y;

	real3 glCoordX = (2.0f * static_cast<real3>(mouseX)) / screenWidth - 1.0f;
	real3 glCoordY = -((2.0f * static_cast<real3>(mouseY)) / screenHeight - 1.0f);
	vec4 clipCoords = vec4(glCoordX, glCoordY, -1.0f, 1.0f);

	float proj[16];
	bx::mtxProj(proj, cameraZoomVl, screenWidth / screenHeight, 0.01f, 10000.0f, bgfx::getCaps()->homogeneousDepth);

	mat4 invertedProjection = glm::inverse(glm::make_mat4(proj));
	vec4 eyeCoords = invertedProjection * clipCoords;
	mat4 invertedView = glm::inverse(glm::lookAtLH(vec3(0.0f), cameraFront, cameraUp));
	vec4 rayWorld = invertedView * eyeCoords;
	vec3 mouseRay = vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay = glm::normalize(mouseRay);

	return cameraPos + mouseRay * distance;
}

vec3 eTools::Get3DRaycastedPosition(const real3 X, const real3 Y, const real3 screenWidth, const real3 screenHeight,
	vec3 cameraPos, vec3 cameraUp, vec3 cameraFront, real3 cameraZoomVl, const real3 distance)
{
	real3 glCoordX = (2.0f * X) / screenWidth - 1.0f;
	real3 glCoordY = -((2.0f * Y) / screenHeight - 1.0f);
	vec4 clipCoords = vec4(glCoordX, glCoordY, -1.0f, 1.0f);

	float proj[16];
	bx::mtxProj(proj, cameraZoomVl, screenWidth / screenHeight, 0.01f, 10000.0f, bgfx::getCaps()->homogeneousDepth);

	mat4 invertedProjection = glm::inverse(glm::make_mat4(proj));
	vec4 eyeCoords = invertedProjection * clipCoords;
	mat4 invertedView = glm::inverse(glm::lookAtLH(vec3(0.0f), cameraFront, cameraUp));
	vec4 rayWorld = invertedView * eyeCoords;
	vec3 mouseRay = vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay = glm::normalize(mouseRay);

	return cameraPos + mouseRay * distance;
}

vec2 eTools::Get2DCoordFrom3DSpace(vec3 pos, const real3 screenWidth, const real3 screenHeight, vec3 cameraPos,
	vec3 cameraFront, vec3 cameraUp, real3 cameraZoomVl, const bool followScreen, vec2 size)
{
	float proj[16];
	bx::mtxProj(proj, cameraZoomVl, screenWidth / screenHeight, 0.01f, 10000.0f, bgfx::getCaps()->homogeneousDepth);
	mat4 projection = glm::make_mat4(proj);
	mat4 view = glm::lookAtLH(cameraPos, cameraPos + cameraFront, cameraUp);

	vec4 clipSpace = projection * (view * vec4(pos, 1.0f));
	vec3 ndcSpacePos = vec3(clipSpace.x, clipSpace.y, clipSpace.z) / clipSpace.w;
	vec2 windowSpacePos = vec2(((ndcSpacePos.x + 1.0) / 2.0) * screenWidth , ((1.0 - ndcSpacePos.y) / 2.0) * screenHeight );

	if (followScreen)
	{
		if (windowSpacePos.x >= screenWidth - size.x)
			windowSpacePos.x = screenWidth - size.x;
		if (windowSpacePos.x <= 0.0f)
			windowSpacePos.x = 0.0f;

		if (windowSpacePos.y >= screenHeight - size.y)
			windowSpacePos.y = screenHeight - size.y;
		if (windowSpacePos.y <= 0.0f)
			windowSpacePos.y = 0.0f;
	}

	if (!(-clipSpace.w <= ndcSpacePos.x, ndcSpacePos.y, ndcSpacePos.z <= clipSpace.w))
		windowSpacePos = vec2(9999.0f);

	if (pos.x == 0.0f)
		eConsole::PrintLog("WARNING::eTools::Get2DCoordFrom3DSpace", "Position.X can't be 0.0f!");
	return windowSpacePos;
}

const bool eTools::Compare(eTransformation3D first, eTransformation3D last)
{
	bool rtVl = true;
	if (first.mPosition != last.mPosition)
		rtVl = false;
	else if (first.mRotation != last.mRotation)
		rtVl = false;
	else if (first.mScale != last.mScale)
		rtVl = false;
	return rtVl;
}

const bool eTools::Compare(vec3 first, vec3 last)
{
	if (first == last)
		return true;
	return false;
}
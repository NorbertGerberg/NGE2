/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef AUDIO_HPP
#define AUDIO_HPP
#include "Types.hpp"
#include "String.hpp"
#include "Base.hpp"
#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_openmpt.h>
#include "AudioSrc.hpp"

//! Audio class for loading and managing sound
//! \ingroup Engine
class eAudio : public eBase
{
public:
	~eAudio();
	bool Initialize();
	void Unload();

	eAudioCore* GetCore();
	void Seek(eAudioSrc* obj, real6 seconds);
	void FadeSwitch(eAudioSrc* begin, eAudioSrc* last, real6 fadeOutTimeSeconds = 2.0, real6 fadeInTimeSeconds = 1.5, real3 endVolume = 1.0f);
	void CancelFade(eAudioSrc* obj);

	void LoadFile(eAudioSrc* obj, eString filepath);
	void LoadMem(eAudioSrc* obj, eString& file);

	void ReleaseSource(eAudioSrc* obj);

private:
	eAudioCore* mSoloud;
};
#endif
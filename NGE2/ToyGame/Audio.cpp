/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Audio.hpp"
#include <Console.hpp>

eAudio::~eAudio()
{
	Unload();
}

bool eAudio::Initialize()
{
	mSoloud = new eAudioCore();
	eAudioErr err = mSoloud->init();
	if (err != SoLoud::SO_NO_ERROR)
	{
		eConsole::PrintLog("ERROR::eAudio::Initialize", "Failed to initialize audio engine!");
		eConsole::WriteToDisk();
		return false;
	}
	return true;
}

void eAudio::Unload()
{
	if (mSoloud)
	{
		mSoloud->deinit();
		delete mSoloud;
	}
}

eAudioCore* eAudio::GetCore()
{
	return mSoloud;
}

void eAudio::Seek(eAudioSrc* obj, real6 seconds)
{
	mSoloud->setPause(obj->mHandle, true);
	mSoloud->seek(obj->mHandle, seconds);
	mSoloud->setPause(obj->mHandle, false);
}

void eAudio::FadeSwitch(eAudioSrc* begin, eAudioSrc* last, real6 fadeOutTimeSeconds, real6 fadeInTimeSeconds, real3 endVolume)
{
	mSoloud->fadeVolume(begin->mHandle, 0, fadeOutTimeSeconds);
	mSoloud->scheduleStop(begin->mHandle, fadeOutTimeSeconds);
	last->mHandle = mSoloud->play(last->mSource, 0.0f);
	mSoloud->setProtectVoice(last->mHandle, true);
	mSoloud->fadeVolume(last->mHandle, endVolume, fadeInTimeSeconds);
}

void eAudio::CancelFade(eAudioSrc* obj)
{
	mSoloud->setVolume(obj->mHandle, mSoloud->getVolume(obj->mHandle));
}

void eAudio::LoadFile(eAudioSrc* obj, eString filepath)
{
	if (obj->mSource.load(filepath.Get()) != SoLoud::SO_NO_ERROR)
	{
		eConsole::PrintLog("ERROR::eAudio::LoadFile", "Failed to load '" + filepath.GetStrg() + "' file!");
		eConsole::WriteToDisk();
	}
}

void eAudio::LoadMem(eAudioSrc* obj, eString file)
{
	if (obj->mSource.loadMem(reinterpret_cast<const unsigned char*>(file.Get()), file.Lenght(), false, false) != SoLoud::SO_NO_ERROR)
	{
		eConsole::PrintLog("ERROR::eAudio::LoadMem", "Failed to load audio file!");
		eConsole::WriteToDisk();
	}
}

void eAudio::ReleaseSource(eAudioSrc* obj)
{
	obj->mSource.stop();
	delete obj;
}
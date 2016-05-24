#include "stdafx.h"
#include "SoundManager.h"
#include "config.h"



bool CSoundManager::initialize(HWND hwnd)
{
	bool result = true;

	CSound* crashSound = new CSound();
	result = crashSound->Initialize(hwnd, soundCollision);
	crashSound->setSoundKind(CSound::SoundKind::CRASH_SCOUND);

	m_soundVector.push_back(crashSound);



	CSound* startBackgroundSound = new CSound();
	result = startBackgroundSound->Initialize(hwnd, soundStart);
	startBackgroundSound->setSoundKind(CSound::SoundKind::START_BACKGROUND_SOUND);

	m_soundVector.push_back(startBackgroundSound);



	CSound* resultBackgroundSound = new CSound();
	result = resultBackgroundSound->Initialize(hwnd, soundResult);
	resultBackgroundSound->setSoundKind(CSound::SoundKind::RESULT_BACKGROUND_SOUND);

	m_soundVector.push_back(resultBackgroundSound);

	return result;
}

void CSoundManager::play(CSound::SoundKind soundkind, bool isLoop)
{
	for (auto& i : m_soundVector)
	{
		if (i->getSoundKind() == soundkind)
		{
			i->play(isLoop);
		}
	}
}

void CSoundManager::stop(CSound::SoundKind soundkind)
{
	for (auto& i : m_soundVector)
	{
		if (i->getSoundKind() == soundkind)
		{
			i->stop();
		}
	}
}

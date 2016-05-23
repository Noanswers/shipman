#include "stdafx.h"
#include "SoundManager.h"
#include "config.h"



bool CSoundManager::initialize(HWND hwnd)
{
	bool result = true;

	if (!m_collisionSound)
	{
		m_collisionSound = new CSound();
		result = m_collisionSound->Initialize(hwnd, soundCollision);
	}

	if (!m_startBackGroundSound)
	{
		m_startBackGroundSound = new CSound();
		result = m_startBackGroundSound->Initialize(hwnd, soundStart);
	}

	if (!m_resultSound)
	{
		m_resultSound = new CSound();
		result = m_resultSound->Initialize(hwnd, soundResult);
	}
		
	return result;
}


void CSoundManager::playStartBackGroundSound()
{
	m_startBackGroundSound->play(true);
}

void CSoundManager::playCollisionSound()
{
	m_collisionSound->play(false);
}

void CSoundManager::playResultSound()
{
	m_resultSound->play(false);
}

void CSoundManager::stopStartBackGroundSound()
{
	m_startBackGroundSound->stop();
}
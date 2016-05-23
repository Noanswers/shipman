#pragma once
#include "Singleton.h"
#include "Sound.h"

class CSoundManager : public CSingleton<CSoundManager>
{
public:
	CSoundManager() = default;
	~CSoundManager() = default;
	CSoundManager(CSoundManager&) = delete;

	void DestorySingleton() override
	{

	}


	bool initialize(HWND hwnd);

	void playStartBackGroundSound();
	void playCollisionSound();
	void playResultSound();

	void stopStartBackGroundSound();


private:
	CSound* m_startBackGroundSound = nullptr;
	CSound* m_collisionSound = nullptr;
	CSound* m_resultSound = nullptr;

};


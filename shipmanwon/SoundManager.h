#pragma once
#include "Singleton.h"
#include "Sound.h"
#include <vector>

class CSoundManager : public CSingleton<CSoundManager>
{
public:
	
	CSoundManager() = default;
	~CSoundManager() = default;
	CSoundManager(CSoundManager&) = delete;

	void DestorySingleton() override
	{

	}
	
	void shutdown();

	bool initialize(HWND hwnd);

	void play(CSound::SoundKind soundkind, bool isLoop);
	void stop(CSound::SoundKind soundkind);

private:
	std::vector<CSound*> m_soundVector;

};


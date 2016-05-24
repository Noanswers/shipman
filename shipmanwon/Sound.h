/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////
// Class name: SoundClass
///////////////////////////////////////////////////////////////////////////////
class CSound
{


public:


	CSound();
	~CSound();

	enum class SoundKind
	{
		START_BACKGROUND_SOUND,
		CRASH_SCOUND,
		RESULT_BACKGROUND_SOUND,
		GAME_BACKGROUND_SOUND
	};

	bool Initialize(HWND hwnd, std::wstring soundPath);
	void Shutdown();
	bool play(bool isLoop); 
	bool stop();

	void setSoundKind(SoundKind set)
	{
		soundkind = set;
	}
	SoundKind getSoundKind()
	{
		return soundkind;
	}

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(std::wstring filename, IDirectSoundBuffer8 ** secondaryBuffer);

	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile(bool isLoop);

	SoundKind soundkind;


	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	

	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer1;
};

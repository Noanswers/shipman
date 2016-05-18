#pragma once
#include "PlayerObject.h"

struct KeySetting
{
	unsigned int right;
	unsigned int left;
	unsigned int up;
	unsigned int down;
	unsigned int start;
	unsigned int cancel;
	unsigned int boost;
};

class CPlayerData
{
public:
	CPlayerData() = default;
	//CPlayerData(CPlayerObject* pawn) = default;
	//	//:MyPawn(pawn) {};
	CPlayerData(const CPlayerData&) = default;
	~CPlayerData()
	{
		/*delete MyPawn;
		MyPawn = nullptr;*/
	}

	void initialize();
	//void setPawnPosition(float x, float y, float z);

	KeySetting getPlayerKeySetting() const;

	void setPlayerKeyRight(unsigned int key);
	void setPlayerKeyLeft(unsigned int key);
	void setPlayerKeyUp(unsigned int key);
	void setPlayerKeyDown(unsigned int key);
	void setPlayerKeyStart(unsigned int key);
	void setPlayerKeyCancel(unsigned int key);
	
private:
	//CPlayerObject* MyPawn = nullptr;
	KeySetting PlayerKey;	
};


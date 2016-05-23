#pragma once
#include "MyObject.h"
#include <vector>

class CPlayerObject : public CMyObject
{
public:
	CPlayerObject() = default;
	CPlayerObject(const CPlayerObject&) = default;
	~CPlayerObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;

	//원기둥 그리는 함수
	void	createCylinder(void);
	void	createTopCap(void);
	void	createBottomCap(void);

	void	dropDown(float speed); //필드 밖으로 나갔을 때 떨어지는 함수

	//충돌체크
	inline float	getRadius() { return bottomRadius; }
	bool	isCollisionPlayer(CPlayerObject * enemy);

	inline int getPlayerNumber() {	return playerNumber; }
	inline void setPlayerNumber(int set) {	playerNumber = set;	}

	inline bool getOutPlayer()	{ return isOutPlayer; }
	inline void setOutPlayer(bool set) { isOutPlayer = set; }
	
private:
	//원기둥 그리는 변수
	float bottomRadius = 0.3f;
	float topRadius = 0.3f;
	float height = 0.2f;
	UINT sliceCount = 100;
	UINT stackCount = 5;

	//플레이어 정보
	float speed = 0.01f;
	int playerNumber = 1;
	bool isOutPlayer = false;
};
	


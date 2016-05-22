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
	/*bool	renderObject(
		ID3D11DeviceContext* deviceContext, 
		std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc
		) override;*/

	void	createCylinder(void);
	void	createTopCap(void);
	void	createBottomCap(void);

	float	getRadious() 
	{
		return bottomRadius;
	}
	bool	isCollisionPlayer(CPlayerObject * enemy);



	int GetplayerNumber()
	{
		return playerNumber;
	}

	void SetPlayerNumber(int set)
	{
		playerNumber = set;
	}


	bool GetOutPlayer()
	{
		return isOutPlayer;
	}

	void SetOutPlayer(bool set)
	{
		isOutPlayer = set;
	}
	
private:
	//원기둥 그리는 변수들
	float bottomRadius = 0.3f;
	float topRadius = 0.3f;
	float height = 0.2f;
	UINT sliceCount = 100;
	UINT stackCount = 5;
	
	////////////////플레이어 정보
	float speed = 0.01f;

	int playerNumber = 1;
	bool isOutPlayer = false;
};


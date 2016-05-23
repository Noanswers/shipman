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

	//����� �׸��� �Լ�
	void	createCylinder(void);
	void	createTopCap(void);
	void	createBottomCap(void);

	void	dropDown(float speed); //�ʵ� ������ ������ �� �������� �Լ�

	//�浹üũ
	inline float	getRadius() { return bottomRadius; }
	bool	isCollisionPlayer(CPlayerObject * enemy);

	inline int getPlayerNumber() {	return playerNumber; }
	inline void setPlayerNumber(int set) {	playerNumber = set;	}

	inline bool getOutPlayer()	{ return isOutPlayer; }
	inline void setOutPlayer(bool set) { isOutPlayer = set; }
	
private:
	//����� �׸��� ����
	float bottomRadius = 0.3f;
	float topRadius = 0.3f;
	float height = 0.2f;
	UINT sliceCount = 100;
	UINT stackCount = 5;

	//�÷��̾� ����
	float speed = 0.01f;
	int playerNumber = 1;
	bool isOutPlayer = false;
};
	


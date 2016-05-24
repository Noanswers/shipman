#pragma once
#include "MyScene.h"

class CGameScene : public CMyScene
{
public:
	CGameScene() = default;
	CGameScene(const CGameScene&) = default;
	~CGameScene() = default;

	void initialize() override;
};


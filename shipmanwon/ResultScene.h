#pragma once
#include "MyScene.h"
class CResultScene : public CMyScene
{
public:
	CResultScene() = default;
	CResultScene(const CResultScene&) = default;
	~CResultScene() = default;

	void initialize() override;
};


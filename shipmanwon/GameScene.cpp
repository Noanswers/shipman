#include "stdafx.h"
#include "GameScene.h"
#include "StageObject.h"

void CGameScene::initialize()
{
	setSceneColor(0.0f, 0.28f, 0.67f, 1.0f); //cobalt blue

	CStageObject* stage = new CStageObject();
	pushBack(stage, 10);

	//stage->setTranslate(3.0f, 0.0f, 0.0f);
}
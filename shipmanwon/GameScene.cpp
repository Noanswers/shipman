#include "stdafx.h"
#include "GameScene.h"
#include "StageObject.h"

void CGameScene::initialize()
{
	setSceneColor(0.2f, 0.2f, 0.2f, 1.0f);

	CStageObject* stage = new CStageObject();
	pushBack(stage, 10);

	stage->setTranslate(3.0f, 0.0f, 0.0f);
}
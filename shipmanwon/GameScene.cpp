#include "stdafx.h"
#include "GameScene.h"
#include "StageObject.h"

void CGameScene::initialize()
{
	setSceneColor(0.6f, 1.0f, 0.6f, 1.0f);

	CStageObject* stage = new CStageObject();
	pushBack(stage, 10);

	//stage->setTranslate(3.0f, 0.0f, 0.0f);
}
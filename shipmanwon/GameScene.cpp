#include "stdafx.h"
#include "GameScene.h"
#include "StageObject.h"

void CGameScene::initialize()
{
	setSceneColor(0.2f, 0.2f, 0.2f, 1.0f);

	CStageObject* stage = new CStageObject();
	pushBack(stage, 10);
}

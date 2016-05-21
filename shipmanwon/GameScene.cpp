#include "stdafx.h"
#include "GameScene.h"
#include "StageObject.h"
#include "SkyObject.h"

void CGameScene::initialize()
{
	setSceneColor(0.6f, 1.0f, 0.6f, 1.0f);

	CMyObject* sky = new CSkyObject();
	pushBack(sky, 10);
}

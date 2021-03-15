//テスト開発用
#pragma once
#include "GameBaseScene.h"

class Stage_Test :public GameBaseScene {
public:
	Stage_Test();
	~Stage_Test();
	bool ClearChecker();
	void GameClear();
};
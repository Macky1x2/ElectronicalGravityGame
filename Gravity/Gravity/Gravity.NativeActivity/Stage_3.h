#pragma once
#include "GameBaseScene.h"

class Stage_3 :public GameBaseScene {
	int clear_count;
public:
	Stage_3();
	~Stage_3();
	bool ClearChecker();
	void GameClear();
};
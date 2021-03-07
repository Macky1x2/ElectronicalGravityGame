#pragma once
#include "GameBaseScene.h"

class Stage_2 :public GameBaseScene {
	double check_radian, pre_radian;
public:
	Stage_2();
	~Stage_2();
	bool ClearChecker();
	void GameClear();
};
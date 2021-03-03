#pragma once
#include "DxLib.h"
#include "define.h"
#include "SceneBase.h"

class GameClearScene :public SceneBase {
	int star, starGHandle;
public:
	GameClearScene(int);
	~GameClearScene();
	void Update();
	void Draw()const;
};
#pragma once
#include "GameBaseScene.h"

class GameClearScene :public SceneBase {
	int star, starGHandle;
public:
	GameClearScene(int);
	~GameClearScene();
	void Update();
	void Draw()const;
	void ReloadFunction(void);
};
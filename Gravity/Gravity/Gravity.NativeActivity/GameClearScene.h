#pragma once
#include "GameBaseScene.h"

class GameClearScene :public SceneBase {
	int star, starGHandle;
	shared_ptr<SquareButton> go_stage_select_button;
public:
	GameClearScene(int);
	~GameClearScene();
	void Update();
	void Draw()const;
	void ReloadFunction(void);
};
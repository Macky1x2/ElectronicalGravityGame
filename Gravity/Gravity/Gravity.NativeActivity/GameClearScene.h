#pragma once
#include "GameBaseScene.h"

class GameClearScene :public SceneBase {
	int star, starGHandle[3];
	int phase;
	int fade_in, fade_in_speed, fade_out, fade_out_speed;
	shared_ptr<SquareButton> go_stage_select_button;
public:
	GameClearScene(int);
	~GameClearScene();
	void Update();
	void Draw()const;
	void Draw_Objects()const;
	void ReloadFunction(void);
};
#pragma once
#include "SceneManager.h"

class TitleScene :public SceneBase {
	int Tap_THandle;
	int Tap_Color;
	int pre_touch_x, pre_touch_y;
	bool go_stage_select_checker, pre_touch_checker;
	int phase, fade_out, fade_out_speed;
public:
	TitleScene();
	~TitleScene();
	void Update();
	void Draw()const;
	void Draw_Objects()const;
	void ReloadFunction(void);
};
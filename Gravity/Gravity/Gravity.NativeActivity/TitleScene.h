#pragma once
#include "SceneManager.h"
#include "Operation.h"

class TitleScene :public SceneBase {
	int Tap_Color;
	bool go_stage_select_checker, pre_touch_checker;
	int phase, fade_out, fade_out_speed;
	std::shared_ptr<SquareButton> start_button;
public:
	TitleScene();
	~TitleScene();
	void Update();
	void Draw()const;
	void Draw_Objects()const;
	void ReloadFunction(void);
};
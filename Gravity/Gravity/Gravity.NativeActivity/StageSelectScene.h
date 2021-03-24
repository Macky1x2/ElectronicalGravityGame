#pragma once
#include "TitleScene.h"

#define BUTTON_NUM 5

class StageSelectScene :public SceneBase {
	int situation, phase;
	int explain_color;
	int fade_in, fade_in_speed, fade_out, fade_out_speed;
	int board_x[2], board_y[2], boardGHandle, board_fade_in, board_fade_out, board_phase, board_v, board_accel;
	double board_size;
	string stage_title[100], clear_terms[100], star1_terms[100], star2_terms[100], star3_terms[100];
	std::shared_ptr<SquareButton> stage_button[5];
	std::shared_ptr<SquareButton> start_button;
	std::shared_ptr<ReverseSquareButton> select_cancel_button;
public:
	StageSelectScene();
	~StageSelectScene();
	void Update();
	void Draw()const;
	void Draw_Objects()const;
	void Draw_Explain()const;
	void ReloadFunction(void);
};
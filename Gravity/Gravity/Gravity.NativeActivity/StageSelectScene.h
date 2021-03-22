#pragma once
#include "TitleScene.h"
#include "Operation.h"

#define BUTTON_NUM 5

class StageSelectScene :public SceneBase {
	int situation;
	int explainTHandle;
	int explain_color;
	string stage_title[100], clear_terms[100], star1_terms[100], star2_terms[100], star3_terms[100];
	std::shared_ptr<SquareButton> stage_button[5];
	std::shared_ptr<SquareButton> start_button;
	std::shared_ptr<ReverseSquareButton> select_cancel_button;
public:
	StageSelectScene();
	~StageSelectScene();
	void Update();
	void Draw()const;
	void ReloadFunction(void);
};
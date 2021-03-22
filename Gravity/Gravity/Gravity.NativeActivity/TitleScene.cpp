#include "TitleScene.h"
#include "StageSelectScene.h"

SceneBase* Scene_pointer_for_Reload;

TitleScene::TitleScene() {
	Scene_pointer_for_Reload = this;
	Tap_THandle = CreateFontToHandle(NULL, 40, 5, DX_FONTTYPE_NORMAL);
	Tap_Color = GetColor(255, 255, 255);
	go_stage_select_checker = false, pre_touch_checker = false;
}

TitleScene::~TitleScene() {
	DeleteFontToHandle(Tap_THandle);
}

void TitleScene::Update() {
	if (GetTouchInputNum() == 1) {
		int x, y;
		GetTouchInput(0, &x, &y, NULL, NULL);
		pre_touch_x = x;
		pre_touch_y = y;
		if (!pre_touch_checker && y <= 700) {
			go_stage_select_checker = true;
		}
		pre_touch_checker = true;
	}
	else {
		if (go_stage_select_checker) {
			if (pre_touch_y <= 700 && GetTouchInputNum() == 0) {
				nextScene = make_shared<StageSelectScene>();
			}
			else {
				go_stage_select_checker = false;
			}
		}
		pre_touch_checker = false;
	}
}

void TitleScene::Draw()const {
	DrawBox(0, 0, 1081, 701, GetColor(0, 0, 255), TRUE);
	DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(Tap_THandle, "TAP TO START") / 2), 350, "TAP TO START", Tap_Color, Tap_THandle);
}

void TitleScene::ReloadFunction(void) {
	ReloadFileGraphAll();						// ファイルから読み込んだ画像を復元する
	Tap_THandle = CreateFontToHandle(NULL, 40, 5, DX_FONTTYPE_NORMAL);
}
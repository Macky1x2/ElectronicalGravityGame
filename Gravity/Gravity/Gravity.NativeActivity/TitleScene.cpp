#include "TitleScene.h"
#include "StageSelectScene.h"

SceneBase* Scene_pointer_for_Reload;
extern int note_pageGHandle, page1_turnoverGHandle;

TitleScene::TitleScene() {
	Scene_pointer_for_Reload = this;
	Tap_THandle = CreateFontToHandle(NULL, 40, 5, DX_FONTTYPE_NORMAL);
	Tap_Color = GetColor(255, 255, 255);
	go_stage_select_checker = false, pre_touch_checker = false;
	phase = 0;
}

TitleScene::~TitleScene() {
	DeleteFontToHandle(Tap_THandle);
}

void TitleScene::Update() {
	if (phase == 0) {
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
					phase = 1;
					PlayMovieToGraph(page1_turnoverGHandle);
				}
				else {
					go_stage_select_checker = false;
				}
			}
			pre_touch_checker = false;
		}
	}
	else if (phase == 1) {
		//1ページめくりアニメーションが終了しているならば
		if (GetMovieStateToGraph(page1_turnoverGHandle) == 0) {
			nextScene = make_shared<StageSelectScene>();
		}
	}
}

void TitleScene::Draw()const {
	DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
	if (GetMovieStateToGraph(page1_turnoverGHandle) == 1) {
		DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0, 1.5, 0, page1_turnoverGHandle, TRUE, FALSE);
	}
	DrawBox(0, 0, 1081, 701, GetColor(0, 0, 255), TRUE);
	DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(Tap_THandle, "TAP TO START") / 2), 350, "TAP TO START", Tap_Color, Tap_THandle);
}

void TitleScene::ReloadFunction(void) {
	ReloadFileGraphAll();						// ファイルから読み込んだ画像を復元する
	Tap_THandle = CreateFontToHandle(NULL, 40, 5, DX_FONTTYPE_NORMAL);
}
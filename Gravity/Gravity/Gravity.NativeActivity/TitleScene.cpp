#include "TitleScene.h"
#include "StageSelectScene.h"

SceneBase* Scene_pointer_for_Reload;
extern int note_pageGHandle, page1_turnoverGHandle, pagemany_turnoverGHandle, reverse_page1_turnoverGHandle, reverse_pagemany_turnoverGHandle;

TitleScene::TitleScene() {
	Scene_pointer_for_Reload = this;
	Tap_THandle = CreateFontToHandle(NULL, 40, 5, DX_FONTTYPE_NORMAL);
	Tap_Color = GetColor(255, 255, 255);
	go_stage_select_checker = false, pre_touch_checker = false;
	phase = 0;
	fade_out = 255;
	fade_out_speed = 17;
}

TitleScene::~TitleScene() {
	DeleteFontToHandle(Tap_THandle);
}

void TitleScene::Update() {
	//phage==0:通常時, phase==1:シーンチェンジ時
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
					SetAlwaysRunFlag(TRUE);
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
		if (fade_out > 0) {
			fade_out -= fade_out_speed;
		}
		//1ページめくりアニメーションが終了しているならば
		if (GetMovieStateToGraph(page1_turnoverGHandle) == 0) {
			SetAlwaysRunFlag(FALSE);
			SeekMovieToGraph(page1_turnoverGHandle, 0);
			nextScene = make_shared<StageSelectScene>();
		}
	}
}

void TitleScene::Draw()const {
	if (phase == 0) {
		DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
		Draw_Objects();
	}
	else if (phase == 1) {
		if (GetMovieStateToGraph(page1_turnoverGHandle) == 1) {
			DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0, 1.5, 0, page1_turnoverGHandle, TRUE, FALSE);
		}
		else {
			DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);
		}
		if (fade_out > 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_out);
			Draw_Objects();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void TitleScene::Draw_Objects()const {
	DrawBox(0, 0, 1081, 701, GetColor(0, 0, 255), TRUE);
	DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(Tap_THandle, "TAP TO START") / 2), 350, "TAP TO START", Tap_Color, Tap_THandle);
}

void TitleScene::ReloadFunction(void) {
	ReloadFileGraphAll();						// ファイルから読み込んだ画像を復元する
	Tap_THandle = CreateFontToHandle(NULL, 40, 5, DX_FONTTYPE_NORMAL);
	//動画ハンドル完全復元
	if (GetMovieStateToGraph(page1_turnoverGHandle) == 0) {
		page1_turnoverGHandle = LoadGraph("movie\\1page_turnover.ogv");
	}
	if (GetMovieStateToGraph(pagemany_turnoverGHandle) == 0) {
		pagemany_turnoverGHandle = LoadGraph("movie\\manypages_turnover.ogv");
	}
	if (GetMovieStateToGraph(reverse_page1_turnoverGHandle) == 0) {
		reverse_page1_turnoverGHandle = LoadGraph("movie\\reverse_1page_turnover.ogv");
	}
	if (GetMovieStateToGraph(reverse_pagemany_turnoverGHandle) == 0) {
		reverse_pagemany_turnoverGHandle = LoadGraph("movie\\reverse_manypages_turnover.ogv");
	}
}
#include "TitleScene.h"
#include "StageSelectScene.h"

SceneBase* Scene_pointer_for_Reload;
extern int note_pageGHandle, page1_turnoverGHandle, pagemany_turnoverGHandle, reverse_page1_turnoverGHandle, reverse_pagemany_turnoverGHandle;
extern int page_1turnoverSH, page_manyturnoverSH, page_1turnover_reverseSH, page_manyturnover_reverseSH;
extern int makibaTH_S128_T10, makibaTH_S64_T7;

TitleScene::TitleScene() {
	Scene_pointer_for_Reload = this;
	Tap_Color = GetColor(0, 0, 0);
	go_stage_select_checker = false, pre_touch_checker = false;
	phase = 0;
	fade_out = 255;
	fade_out_speed = 17;
	//ボタン
	start_button = std::make_shared<SquareButton>(200, 750, 680, 250);
}

TitleScene::~TitleScene() {
	if (start_button) {
		start_button.reset();
	}
}

void TitleScene::Update() {
	//phage==0:通常時, phase==1:シーンチェンジ時
	if (phase == 0) {
		if (start_button->Checker_specific_place_touch_in_out()) {
			phase = 1;
			SetAlwaysRunFlag(TRUE);
			PlaySoundMem(page_1turnoverSH, DX_PLAYTYPE_BACK, TRUE);
			PlayMovieToGraph(page1_turnoverGHandle);
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
	DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(makibaTH_S128_T10, "Particle Puzzle\n訳して粒子パズル!") / 2), 200, "Particle Puzzle\n訳して粒子パズル!", Tap_Color, makibaTH_S128_T10);
	DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(makibaTH_S128_T10, "はじめる") / 2), 750, "はじめる", Tap_Color, makibaTH_S128_T10);
}

void TitleScene::ReloadFunction(void) {
	
}
#include "GameClearScene.h"
#include "StageSelectScene.h"

extern SceneBase* Scene_pointer_for_Reload;
extern int note_pageGHandle, page1_turnoverGHandle, pagemany_turnoverGHandle, reverse_page1_turnoverGHandle, reverse_pagemany_turnoverGHandle;
extern int page_1turnoverSH, page_manyturnoverSH, page_1turnover_reverseSH, page_manyturnover_reverseSH;
extern int makibaTH_S128_T10, makibaTH_S64_T7;

GameClearScene::GameClearScene(int _star) {
	phase = 0;
	fade_in = 0;
	fade_in_speed = 17;
	fade_out = 255;
	fade_out_speed = 17;
	star = _star;
	starGHandle[0] = LoadGraph("graph\\star\\star_1.png");
	starGHandle[1] = LoadGraph("graph\\star\\star_2.png");
	starGHandle[2] = LoadGraph("graph\\star\\star_3.png");
	Scene_pointer_for_Reload = this;
	go_stage_select_button = make_shared<SquareButton>(400, 1480, 640, 200);
}

GameClearScene::~GameClearScene() {
	for (int i = 0; i < 3; i++) {
		DeleteGraph(starGHandle[i]);
	}
}

void GameClearScene::Update() {
	if (phase == 0) {
		if (fade_in < 255) {
			fade_in += fade_in_speed;
		}
		if (go_stage_select_button->Checker_specific_place_touch_in_out()) {
			phase = 1;
			SetAlwaysRunFlag(TRUE);
			PlaySoundMem(page_manyturnover_reverseSH, DX_PLAYTYPE_BACK, TRUE);
			PlayMovieToGraph(reverse_pagemany_turnoverGHandle);
		}
	}
	else if (phase == 1) {
		if (fade_out > 0) {
			fade_out -= fade_out_speed;
		}
		if (GetMovieStateToGraph(reverse_pagemany_turnoverGHandle) == 0) {
			SetAlwaysRunFlag(FALSE);
			SeekMovieToGraph(reverse_pagemany_turnoverGHandle, 0);
			nextScene = make_shared<StageSelectScene>();
		}
	}
}

void GameClearScene::Draw()const {
	if (phase == 0) {
		DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
		if (fade_in < 255) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_in);
			Draw_Objects();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			Draw_Objects();
		}
	}
	else if (phase == 1) {
		if (GetMovieStateToGraph(reverse_pagemany_turnoverGHandle) == 1) {
			DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0, 1.5, 0, reverse_pagemany_turnoverGHandle, TRUE, FALSE);
		}
		else {
			DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
		}
		if (fade_out > 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_out);
			Draw_Objects();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void GameClearScene::Draw_Objects()const {
	//DrawBox(0, 1500, 1081, 1921, GetColor(0, 0, 255), TRUE);
	DrawFormatStringToHandle(400, 1500, GetColor(0, 0, 0), makibaTH_S64_T7, "ステージセレクト\n　　　　 　画面へ");
	for (int i = 0; i < star; i++) {
		DrawRotaGraph(300 * (i + 1), 500, 0.9, 0, starGHandle[i], TRUE, FALSE);
	}
}

void GameClearScene::ReloadFunction(void) {
	
}
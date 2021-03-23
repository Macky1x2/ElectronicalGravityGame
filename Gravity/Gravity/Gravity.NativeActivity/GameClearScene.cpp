﻿#include "GameClearScene.h"
#include "StageSelectScene.h"

extern SceneBase* Scene_pointer_for_Reload;
extern int note_pageGHandle, page1_turnoverGHandle, pagemany_turnoverGHandle, reverse_page1_turnoverGHandle, reverse_pagemany_turnoverGHandle;

GameClearScene::GameClearScene(int _star) {
	phase = 0;
	fade_in = 0;
	fade_in_speed = 17;
	fade_out = 255;
	fade_out_speed = 17;
	star = _star;
	starGHandle = LoadGraph("star.png");
	Scene_pointer_for_Reload = this;
	go_stage_select_button = make_shared<SquareButton>(0, 1500, ANDROID_WIDTH, ANDROID_HEIGHT - 1500);
}

GameClearScene::~GameClearScene() {
	DeleteGraph(starGHandle);
}

void GameClearScene::Update() {
	if (phase == 0) {
		if (fade_in < 255) {
			fade_in += fade_in_speed;
		}
		if (go_stage_select_button->Checker_specific_place_touch_in_out()) {
			phase = 1;
			SetAlwaysRunFlag(TRUE);
			PlayMovieToGraph(reverse_pagemany_turnoverGHandle);
		}
	}
	else if (phase == 1) {
		if (fade_out > 0) {
			fade_out -= fade_out_speed;
		}
		if (GetMovieStateToGraph(reverse_pagemany_turnoverGHandle) == 0) {
			nextScene = make_shared<StageSelectScene>();
		}
	}
}

void GameClearScene::Draw()const {
	if (phase == 0) {
		DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
		if (fade_in < 255) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_in);
			DrawBox(0, 1500, 1081, 1921, GetColor(0, 0, 255), TRUE);
			for (int i = 0; i < star; i++) {
				DrawRotaGraph(300 * (i + 1), 500, 0.5, 0, starGHandle, TRUE, FALSE);
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			DrawBox(0, 1500, 1081, 1921, GetColor(0, 0, 255), TRUE);
			for (int i = 0; i < star; i++) {
				DrawRotaGraph(300 * (i + 1), 500, 0.5, 0, starGHandle, TRUE, FALSE);
			}
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
			DrawBox(0, 1500, 1081, 1921, GetColor(0, 0, 255), TRUE);
			for (int i = 0; i < star; i++) {
				DrawRotaGraph(300 * (i + 1), 500, 0.5, 0, starGHandle, TRUE, FALSE);
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	/*
	DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
	if (GetMovieStateToGraph(reverse_pagemany_turnoverGHandle) == 1) {
		DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0, 1.5, 0, reverse_pagemany_turnoverGHandle, TRUE, FALSE);
	}
	DrawBox(0, 1500, 1081, 1921, GetColor(0, 0, 255), TRUE);
	for (int i = 0; i < star; i++) {
		DrawRotaGraph(300 * (i + 1), 500, 0.5, 0, starGHandle, TRUE, FALSE);
	}*/
}

void GameClearScene::ReloadFunction(void) {
	ReloadFileGraphAll();						// ファイルから読み込んだ画像を復元する
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
#include "StageSelectScene.h"
#include "Stage_1.h"
#include "Stage_2.h"
#include "Stage_3.h"
#include "Stage_4.h"
#include "Stage_Test.h"

extern SceneBase* Scene_pointer_for_Reload;
extern int note_pageGHandle, page1_turnoverGHandle, pagemany_turnoverGHandle, reverse_page1_turnoverGHandle, reverse_pagemany_turnoverGHandle;

StageSelectScene::StageSelectScene() {
	Scene_pointer_for_Reload = this;
	situation = 0;
	phase = 0;
	fade_in = 0;
	fade_in_speed = 17;
	fade_out = 255;
	fade_out_speed = 17;
	explainTHandle = CreateFontToHandle(NULL, 40, 5, DX_FONTTYPE_NORMAL);
	explain_color = GetColor(255, 255, 255);
	//ボタン
	for (int i = 0; i < 5; i++) {
		stage_button[i] = std::make_shared<SquareButton>(100 + 195 * i, 200, 100, 100);
	}
	start_button = std::make_shared<SquareButton>(250, 980, 580, 200);
	select_cancel_button = std::make_shared<ReverseSquareButton>(100, 100, 880, 1180);
	//ステージ開始前画面の説明テキスト//iniファイルロードが良き?
	stage_title[0] = "チュートリアルA", clear_terms[0] = "球(-10)を取得する", star1_terms[0] = "4ショット以内でクリア", star2_terms[0] = "2ショット以内でクリア", star3_terms[0] = "1ショット以内でクリア";
	stage_title[1] = "公転", clear_terms[1] = "大きな球の周りを3週回る", star1_terms[1] = "3ショット以内でクリア", star2_terms[1] = "2ショット以内でクリア", star3_terms[1] = "1ショット以内でクリア";
	stage_title[2] = "分身", clear_terms[2] = "小さな球を50個集める", star1_terms[2] = "30秒以内にクリア", star2_terms[2] = "20秒以内にクリア", star3_terms[2] = "17秒以内にクリア";
	stage_title[3] = "収容", clear_terms[3] = "球(-9999)を取得する", star1_terms[3] = "10ショット以内でクリア", star2_terms[3] = "5ショット以内でクリア", star3_terms[3] = "3ショット以内でクリア";
}

StageSelectScene::~StageSelectScene() {
	for (int i = 0; i < BUTTON_NUM; i++) {
		if (stage_button[i]) {
			stage_button[i].reset();
		}
	}
	if (start_button) {
		start_button.reset();
	}
	if (select_cancel_button) {
		select_cancel_button.reset();
	}
	DeleteFontToHandle(explainTHandle);
}

void StageSelectScene::Update() {
	if (phase == 0) {
		if (fade_in < 255) {
			fade_in += fade_in_speed;
		}
		if (situation == 0) {
			for (int i = 0; i < 5; i++) {
				if (stage_button[i]->Checker_specific_place_touch_in_out()) {
					situation = i + 1;
					break;
				}
			}
		}
		else {
			if (select_cancel_button->Checker_reverse_specific_place_touch_in_out()) {
				situation = 0;
			}
			else if (start_button->Checker_specific_place_touch_in_out()) {
				phase = 1;
				SetAlwaysRunFlag(TRUE);
				PlayMovieToGraph(pagemany_turnoverGHandle);
			}
		}
	}
	else if (phase == 1) {
		if (fade_out > 0) {
			fade_out -= fade_out_speed;
		}
		if (GetMovieStateToGraph(pagemany_turnoverGHandle) == 0) {
			SetAlwaysRunFlag(FALSE);
			SeekMovieToGraph(pagemany_turnoverGHandle, 0);
			switch (situation) {
			case 1:nextScene = make_shared<Stage_1>(); break;
			case 2:nextScene = make_shared<Stage_2>(); break;
			case 3:nextScene = make_shared<Stage_3>(); break;
			case 4:nextScene = make_shared<Stage_4>(); break;
			case 5:nextScene = make_shared<Stage_Test>(); break;
			default:break;
			}
		}
	}
}

void StageSelectScene::Draw()const {
	if (phase == 0) {
		DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
		if (fade_in < 255) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_in);
			for (int i = 0; i < 5; i++) {
				DrawBox(100 + 195 * i, 200, 201 + 195 * i, 301, GetColor(0, 0, 255), TRUE);
			}
			if (situation != 0) {
				DrawBox(100, 100, 981, 1281, GetColor(0, 255, 0), TRUE);
				DrawBox(250, 980, 831, 1181, GetColor(0, 0, 255), TRUE);
				DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "%s", stage_title[situation - 1].c_str()) / 2), 200, explain_color, explainTHandle, "%s", stage_title[situation - 1].c_str());
				DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "クリア条件") / 2), 400, "クリア条件", explain_color, explainTHandle);
				DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "%s", clear_terms[situation - 1].c_str()) / 2), 450, explain_color, explainTHandle, "%s", clear_terms[situation - 1].c_str());
				DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str()) / 2), 750, explain_color, explainTHandle, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str());
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			for (int i = 0; i < 5; i++) {
				DrawBox(100 + 195 * i, 200, 201 + 195 * i, 301, GetColor(0, 0, 255), TRUE);
			}
			if (situation != 0) {
				DrawBox(100, 100, 981, 1281, GetColor(0, 255, 0), TRUE);
				DrawBox(250, 980, 831, 1181, GetColor(0, 0, 255), TRUE);
				DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "%s", stage_title[situation - 1].c_str()) / 2), 200, explain_color, explainTHandle, "%s", stage_title[situation - 1].c_str());
				DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "クリア条件") / 2), 400, "クリア条件", explain_color, explainTHandle);
				DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "%s", clear_terms[situation - 1].c_str()) / 2), 450, explain_color, explainTHandle, "%s", clear_terms[situation - 1].c_str());
				DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str()) / 2), 750, explain_color, explainTHandle, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str());
			}
		}
	}
	else if (phase == 1) {
		if (GetMovieStateToGraph(pagemany_turnoverGHandle) == 1) {
			DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0, 1.5, 0, pagemany_turnoverGHandle, TRUE, FALSE);
		}
		else {
			DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
		}
		if (fade_out > 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_out);
			if (situation != 0) {
				DrawBox(100, 100, 981, 1281, GetColor(0, 255, 0), TRUE);
				DrawBox(250, 980, 831, 1181, GetColor(0, 0, 255), TRUE);
				DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "%s", stage_title[situation - 1].c_str()) / 2), 200, explain_color, explainTHandle, "%s", stage_title[situation - 1].c_str());
				DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "クリア条件") / 2), 400, "クリア条件", explain_color, explainTHandle);
				DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "%s", clear_terms[situation - 1].c_str()) / 2), 450, explain_color, explainTHandle, "%s", clear_terms[situation - 1].c_str());
				DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str()) / 2), 750, explain_color, explainTHandle, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str());
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	/*
	DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
	if (GetMovieStateToGraph(pagemany_turnoverGHandle) == 1) {
		DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0, 1.5, 0, pagemany_turnoverGHandle, TRUE, FALSE);
	}
	for (int i = 0; i < 5; i++) {
		DrawBox(100 + 195 * i, 200, 201 + 195 * i, 301, GetColor(0, 0, 255), TRUE);
	}
	if (situation != 0) {
		DrawBox(100, 100, 981, 1281, GetColor(0, 255, 0), TRUE);
		DrawBox(250, 980, 831, 1181, GetColor(0, 0, 255), TRUE);
		DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "%s", stage_title[situation - 1].c_str()) / 2), 200, explain_color, explainTHandle, "%s", stage_title[situation - 1].c_str());
		DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "クリア条件") / 2), 400, "クリア条件", explain_color, explainTHandle);
		DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "%s", clear_terms[situation - 1].c_str()) / 2), 450, explain_color, explainTHandle, "%s", clear_terms[situation - 1].c_str());
		DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str()) / 2), 750, explain_color, explainTHandle, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str());
	}*/
}

void StageSelectScene::ReloadFunction(void) {
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
#include "StageSelectScene.h"
#include "Stage_1.h"
#include "Stage_2.h"
#include "Stage_3.h"
#include "Stage_4.h"
#include "Stage_Test.h"

extern SceneBase* Scene_pointer_for_Reload;
extern int note_pageGHandle, page1_turnoverGHandle, pagemany_turnoverGHandle, reverse_page1_turnoverGHandle, reverse_pagemany_turnoverGHandle;
extern int makibaTH_S64_T7;

StageSelectScene::StageSelectScene() {
	Scene_pointer_for_Reload = this;
	situation = 0;
	phase = 0;
	board_phase = 0;
	fade_in = 0;
	fade_in_speed = 17;
	fade_out = 255;
	fade_out_speed = 17;
	board_accel = 11;
	board_size = 2.5;
	boardGHandle = LoadGraph("graph\\clipboard.png");
	explain_color = GetColor(0, 0, 0);
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
}

void StageSelectScene::Update() {
	if (phase == 0) {
		if (fade_in < 255) {
			fade_in += fade_in_speed;
		}
		if (situation == 0) {
			for (int i = 0; i < 5; i++) {
				//ステージiを選択したら
				if (stage_button[i]->Checker_specific_place_touch_in_out()) {
					situation = i + 1;
					board_phase = 0;
					break;
				}
			}
		}
		else {
			switch (board_phase) {
			case 0:board_x[0] = -385;
				board_y[0] = ANDROID_HEIGHT / 2;
				board_v = 142;
				board_fade_in = 0;
				board_phase = 1;
				break;
			case 1:
				if (board_x[0] < 540) {
					board_x[0] += board_v;
					board_v -= board_accel;
				}
				else {
					board_phase = 2;
				}
				break;
			case 2:
				if (board_fade_in < 255) {
					board_fade_in += fade_in_speed;
				}
				break;
			case 3:board_v = 142;
				board_fade_out = 255;
				board_phase = 4;
				break;
			case 4:
				if (board_fade_out > 0) {
					board_fade_out -= fade_in_speed;
				}
				if (board_x[0] > -385) {
					board_x[0] -= board_v;
					board_v -= board_accel;
				}
				else {
					situation = 0;
				}
				break;
			default:break;
			}
			if (board_phase == 2 && board_fade_in >= 255) {
				//枠外タッチしたら
				if (select_cancel_button->Checker_reverse_specific_place_touch_in_out()) {
					board_phase = 3;
					//situation = 0;
				}
				else if (start_button->Checker_specific_place_touch_in_out()) {		//スタートボタン押したら
					phase = 1;
					SetAlwaysRunFlag(TRUE);
					PlayMovieToGraph(pagemany_turnoverGHandle);
				}
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
			//アニメーション終了後phage0で選択したステージへgo
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
			Draw_Objects();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			Draw_Objects();
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
			//ステージ説明部分(このときsituation!=0は明らか)
			Draw_Objects();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void StageSelectScene::Draw_Objects()const {
	for (int i = 0; i < 5; i++) {
		DrawBox(100 + 195 * i, 200, 201 + 195 * i, 301, GetColor(0, 0, 255), TRUE);
	}
	if (situation != 0) {
		switch (board_phase) {
		case 1:DrawRotaGraph(board_x[0], board_y[0], board_size, 0, boardGHandle, TRUE, FALSE);
			break;
		case 2:DrawRotaGraph(board_x[0], board_y[0], board_size, 0, boardGHandle, TRUE, FALSE);
			if (board_fade_in < 255) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, board_fade_in);
				Draw_Explain();
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else {
				Draw_Explain();
			}
			break;
		case 3:DrawRotaGraph(board_x[0], board_y[0], board_size, 0, boardGHandle, TRUE, FALSE);
			Draw_Explain();
			break;
		case 4:DrawRotaGraph(board_x[0], board_y[0], board_size, 0, boardGHandle, TRUE, FALSE);
			if (board_fade_out > 0) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, board_fade_out);
				Draw_Explain();
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			break;
		default:break;
		}
	}
}

void StageSelectScene::Draw_Explain()const {
	DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "%s", stage_title[situation - 1].c_str()) / 2), 200, explain_color, makibaTH_S64_T7, "%s", stage_title[situation - 1].c_str());
	DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "クリア条件") / 2), 400, "クリア条件", explain_color, makibaTH_S64_T7);
	DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "%s", clear_terms[situation - 1].c_str()) / 2), 450, explain_color, makibaTH_S64_T7, "%s", clear_terms[situation - 1].c_str());
	DrawFormatStringToHandle(540 - (GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str()) / 2), 750, explain_color, makibaTH_S64_T7, "　　　:%s\n　　　:%s\n　　　:%s", star1_terms[situation - 1].c_str(), star2_terms[situation - 1].c_str(), star3_terms[situation - 1].c_str());
}

void StageSelectScene::ReloadFunction(void) {
	
}
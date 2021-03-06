#include "StageSelectScene.h"
#include "Stage_1.h"
#include "Stage_2.h"
#include "Stage_3.h"
#include "Stage_4.h"
#include "Stage_Test.h"

extern SceneBase* Scene_pointer_for_Reload;

StageSelectScene::StageSelectScene() {
	Scene_pointer_for_Reload = this;
	situation = 0;
	explainTHandle = CreateFontToHandle(NULL, 40, 5, DX_FONTTYPE_NORMAL);
	explain_color = GetColor(255, 255, 255);
}

StageSelectScene::~StageSelectScene() {
	DeleteFontToHandle(explainTHandle);
}

void StageSelectScene::Update() {
	if (situation == 0) {
		if (GetTouchInputNum() == 1) {
			for (int i = 0; i < 5; i++) {
				int tempx, tempy;
				GetTouchInput(0, &tempx, &tempy, NULL, NULL);
				if (100 + 195 * i <= tempx && tempx < 201 + 195 * i && 200 <= tempy && tempy < 301) {
					switch (i) {
					case 0:situation = 1; break;
					case 1:situation = 2; break;
					case 2:situation = 3; break;
					case 3:situation = 4; break;
					case 4:situation = 5; break;
					default:break;
					}
				}
			}
		}
	}
	else{
		if (GetTouchInputNum() == 1) {
			int tempx, tempy;
			GetTouchInput(0, &tempx, &tempy, NULL, NULL);
			if (!(100 <= tempx && tempx < 981 && 100 <= tempy && tempy < 1281)) {
				situation = 0;
			}
			else if (250 <= tempx && tempx < 831 && 980 <= tempy && tempy < 1181) {
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
}

void StageSelectScene::Draw()const {
	for (int i = 0; i < 5; i++) {
		DrawBox(100 + 195 * i, 200, 201 + 195 * i, 301, GetColor(0, 0, 255), TRUE);
	}
	if (situation != 0) {
		DrawBox(100, 100, 981, 1281, GetColor(0, 255, 0), TRUE);
		DrawBox(250, 980, 831, 1181, GetColor(0, 0, 255), TRUE);
		switch (situation) {
		case 1:DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "チュートリアルA") / 2), 200, "チュートリアルA", explain_color, explainTHandle);
			DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "クリア条件") / 2), 400, "クリア条件", explain_color, explainTHandle);
			DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "球(-10)を取得すること") / 2), 500, "球(-10)を取得すること", explain_color, explainTHandle);
			DrawStringToHandle(540 - (GetDrawFormatStringWidthToHandle(explainTHandle, "　　　:1ショット以内でクリア\n　　　:2ショット以内でクリア\n　　　:4ショット以内でクリア") / 2), 800, "　　　:1ショット以内でクリア\n　　　:2ショット以内でクリア\n　　　:4ショット以内でクリア", explain_color, explainTHandle);
			break;
		default:break;
		}
	}
}

void StageSelectScene::ReloadFunction(void) {
	ReloadFileGraphAll();						// ファイルから読み込んだ画像を復元する
}
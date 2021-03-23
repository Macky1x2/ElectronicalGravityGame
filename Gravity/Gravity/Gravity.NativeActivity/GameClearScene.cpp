#include "GameClearScene.h"
#include "StageSelectScene.h"

extern SceneBase* Scene_pointer_for_Reload;
extern int note_pageGHandle, page1_turnoverGHandle;

GameClearScene::GameClearScene(int _star) {
	star = _star;
	starGHandle = LoadGraph("star.png");
	Scene_pointer_for_Reload = this;
	go_stage_select_button = make_shared<SquareButton>(0, 1500, ANDROID_WIDTH, ANDROID_HEIGHT - 1500);
}

GameClearScene::~GameClearScene() {
	DeleteGraph(starGHandle);
}

void GameClearScene::Update() {
	if (go_stage_select_button->Checker_specific_place_touch_in_out()) {
		nextScene = make_shared<StageSelectScene>();
	}
}

void GameClearScene::Draw()const {
	DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
	DrawBox(0, 1500, 1081, 1921, GetColor(0, 0, 255), TRUE);
	for (int i = 0; i < star; i++) {
		DrawRotaGraph(300 * (i + 1), 500, 0.5, 0, starGHandle, TRUE, FALSE);
	}
}

void GameClearScene::ReloadFunction(void) {
	ReloadFileGraphAll();						// ファイルから読み込んだ画像を復元する
}
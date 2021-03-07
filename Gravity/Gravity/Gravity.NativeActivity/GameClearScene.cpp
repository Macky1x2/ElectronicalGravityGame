#include "GameClearScene.h"
#include "StageSelectScene.h"

extern SceneBase* Scene_pointer_for_Reload;

GameClearScene::GameClearScene(int _star) {
	star = _star;
	starGHandle = LoadGraph("star.png");
	Scene_pointer_for_Reload = this;
}

GameClearScene::~GameClearScene() {
	DeleteGraph(starGHandle);
}

void GameClearScene::Update() {
	if (GetTouchInputNum() == 1) {
		int x,y;
		GetTouchInput(0, &x, &y, NULL, NULL);
		if (y >= 1500) {
			nextScene = make_shared<StageSelectScene>();
		}
	}
}

void GameClearScene::Draw()const {
	DrawBox(0, 1500, 1081, 1921, GetColor(0, 0, 255), TRUE);
	for (int i = 0; i < star; i++) {
		DrawRotaGraph(300 * (i + 1), 500, 0.5, 0, starGHandle, TRUE, FALSE);
	}
}

void GameClearScene::ReloadFunction(void) {
	ReloadFileGraphAll();						// ファイルから読み込んだ画像を復元する
}
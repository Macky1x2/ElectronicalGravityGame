#include "GameClearScene.h"
#include "TitleScene.h"

GameClearScene::GameClearScene(int _star) {
	star = _star;
	starGHandle = LoadGraph("star.png");
}

GameClearScene::~GameClearScene() {
	DeleteGraph(starGHandle);
}

void GameClearScene::Update() {
	if (GetTouchInputNum() == 1) {
		int x,y;
		GetTouchInput(0, &x, &y, NULL, NULL);
		if (y >= 1500) {
			nextScene = make_shared<TitleScene>();
		}
	}
}

void GameClearScene::Draw()const {
	DrawBox(0, 1500, 1081, 1921, GetColor(0, 0, 255), TRUE);
	for (int i = 0; i < star; i++) {
		DrawRotaGraph(300 * (i + 1), 500, 0.5, 0, starGHandle, TRUE, FALSE);
	}
}
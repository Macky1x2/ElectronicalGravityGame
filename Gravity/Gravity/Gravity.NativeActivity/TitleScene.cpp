#include "TitleScene.h"
#include "StageSelectScene.h"

TitleScene::TitleScene() {

}

TitleScene::~TitleScene() {

}

void TitleScene::Update() {
	if (GetTouchInputNum() == 1) {
		int x, y;
		GetTouchInput(0, &x, &y, NULL, NULL);
		if (y <= 700) {
			nextScene = make_shared<StageSelectScene>();
		}
	}
}

void TitleScene::Draw()const {
	DrawBox(0, 0, 1081, 701, GetColor(0, 0, 255), TRUE);
}
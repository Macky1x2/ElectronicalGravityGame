#include "TitleScene.h"
#include "TestGameScene.h"

TitleScene::TitleScene() {

}

TitleScene::~TitleScene() {

}

void TitleScene::Update() {
	if (GetTouchInputNum() == 1) {
		int x, y;
		GetTouchInput(0, &x, &y, NULL, NULL);
		if (y <= 700) {
			nextScene = make_shared<TestGameScene>();
		}
	}
}

void TitleScene::Draw()const {
	DrawBox(0, 0, 1081, 701, GetColor(0, 0, 255), TRUE);
}
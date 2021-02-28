#include "TitleScene.h"
#include "TestGameScene.h"

TitleScene::TitleScene() {

}

TitleScene::~TitleScene() {

}

void TitleScene::Update() {
	//画面をタップしたらテストゲームシーンへ
	if (GetTouchInputNum() == 1) {
		nextScene = make_shared<TestGameScene>();
	}
}

void TitleScene::Draw()const {

}
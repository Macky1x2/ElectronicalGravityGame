#include "TitleScene.h"
#include "TestGameScene.h"

TitleScene::TitleScene() {

}

TitleScene::~TitleScene() {

}

void TitleScene::Update() {
	//��ʂ��^�b�v������e�X�g�Q�[���V�[����
	if (GetTouchInputNum() == 1) {
		nextScene = make_shared<TestGameScene>();
	}
}

void TitleScene::Draw()const {

}
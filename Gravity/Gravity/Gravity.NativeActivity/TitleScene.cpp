#include "TitleScene.h"
#include "StageSelectScene.h"

SceneBase* Scene_pointer_for_Reload;

TitleScene::TitleScene() {
	Scene_pointer_for_Reload = this;
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

void TitleScene::ReloadFunction(void) {
	ReloadFileGraphAll();						// ƒtƒ@ƒCƒ‹‚©‚ç“Ç‚İ‚ñ‚¾‰æ‘œ‚ğ•œŒ³‚·‚é
}
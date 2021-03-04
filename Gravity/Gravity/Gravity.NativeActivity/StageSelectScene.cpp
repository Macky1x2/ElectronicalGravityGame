#include "StageSelectScene.h"
#include "Stage_1.h"
#include "Stage_Test.h"

StageSelectScene::StageSelectScene() {

}

StageSelectScene::~StageSelectScene() {

}

void StageSelectScene::Update() {
	if (GetTouchInputNum() == 1) {
		for (int i = 0; i < 5; i++) {
			int tempx, tempy;
			GetTouchInput(0, &tempx, &tempy, NULL, NULL);
			if (100 + 200 * i <= tempx && tempx < 201 + 200 * i && 200 <= tempy && tempy < 301) {
				switch (i) {
				case 0:nextScene = make_shared<Stage_1>(); break;
				case 1:nextScene = make_shared<Stage_1>(); break;
				case 2:nextScene = make_shared<Stage_1>(); break;
				case 3:nextScene = make_shared<Stage_1>(); break;
				case 4:nextScene = make_shared<Stage_Test>(); break;
				default:break;
				}
			}
		}
	}
}

void StageSelectScene::Draw()const {
	for (int i = 0; i < 5; i++) {
		DrawBox(100 + 200 * i, 200, 201 + 200 * i, 301, GetColor(0, 0, 255), TRUE);
	}
}
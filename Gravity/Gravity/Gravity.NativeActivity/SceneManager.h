#pragma once
#include "DxLib.h"
#include "SceneBase.h"
#include <memory>

using namespace std;

class SceneManager {
	shared_ptr<SceneBase> currentScene;
public:
	SceneManager(shared_ptr<SceneBase> initialScene)
		:currentScene(initialScene){}
	void Update() {
		//次のシーンが代入されていれば遷移
		if (currentScene->nextScene) {
			currentScene = move(currentScene->nextScene);
		}
		else {
			currentScene->Update();
		}
	}
	void Draw()const {
		ClearDrawScreen();
		currentScene->Draw();
		ScreenFlip();
	}
};
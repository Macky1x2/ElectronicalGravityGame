#pragma once
#include "SceneBase.h"

using namespace std;

class SceneManager {
	shared_ptr<SceneBase> currentScene;
public:
	SceneManager(shared_ptr<SceneBase> initialScene)
		:currentScene(initialScene) {}
	void Update() {
		//ŽŸ‚ÌƒV[ƒ“‚ª‘ã“ü‚³‚ê‚Ä‚¢‚ê‚Î‘JˆÚ
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
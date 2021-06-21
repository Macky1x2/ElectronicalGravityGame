#include "GameScene.h"

// プログラムは android_main から始まります
int android_main(void)
{
	//初期化
	if (DxLib_Init() == -1){
		return -1;
	}
	//画面サイズ設定
	SetGraphMode(ANDROID_WIDTH, ANDROID_HEIGHT, 32);
	//描画先を裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);

	//ゲーム処理
	SceneManager scene(make_shared<GameScene>());
	while (ProcessMessage() == 0) {
		scene.Update();
		scene.Draw();
	}

	//終了処理
	DxLib_End();
	return 0;
}
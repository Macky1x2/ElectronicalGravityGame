﻿#include "TitleScene.h"

int note_pageGHandle, page1_turnoverGHandle, pagemany_turnoverGHandle, reverse_page1_turnoverGHandle, reverse_pagemany_turnoverGHandle;
extern SceneBase* Scene_pointer_for_Reload;

void ReloadFunctionInGame();

// プログラムは android_main から始まります
int android_main(void)
{
	//初期化
	if (DxLib_Init() == -1){
		return -1;
	}
	SetGraphMode(ANDROID_WIDTH, ANDROID_HEIGHT, 32);
	SetDrawScreen(DX_SCREEN_BACK);
	SetRestoreGraphCallback(ReloadFunctionInGame);
	note_pageGHandle = LoadGraph("graph\\note_page.png");
	page1_turnoverGHandle = LoadGraph("movie\\1page_turnover.ogv");
	pagemany_turnoverGHandle = LoadGraph("movie\\manypages_turnover.ogv");
	reverse_page1_turnoverGHandle = LoadGraph("movie\\reverse_1page_turnover.ogv");
	reverse_pagemany_turnoverGHandle = LoadGraph("movie\\reverse_manypages_turnover.ogv");

	//ゲーム処理
	SceneManager scene(make_shared<TitleScene>());
	while (ProcessMessage() == 0) {
		scene.Update();
		scene.Draw();
	}

	//終了処理
	DeleteGraph(note_pageGHandle);
	DeleteGraph(page1_turnoverGHandle);
	DeleteGraph(pagemany_turnoverGHandle);
	DeleteGraph(reverse_page1_turnoverGHandle);
	DeleteGraph(reverse_pagemany_turnoverGHandle);
	DxLib_End();
	return 0;
}

void ReloadFunctionInGame() {
	if (Scene_pointer_for_Reload) {
		Scene_pointer_for_Reload->ReloadFunction();
	}
}
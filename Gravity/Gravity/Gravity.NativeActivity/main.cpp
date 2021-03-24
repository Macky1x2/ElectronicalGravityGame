#include "TitleScene.h"

int note_pageGHandle, page1_turnoverGHandle, pagemany_turnoverGHandle, reverse_page1_turnoverGHandle, reverse_pagemany_turnoverGHandle;		//背景(どのシーンでも使う),シーンチェンジアニメーションハンドル
int makibaTH_S64_T7;
extern SceneBase* Scene_pointer_for_Reload;

void ReloadFunctionInGame();

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
	//復元関数を登録
	SetRestoreGraphCallback(ReloadFunctionInGame);
	//グローバル変数ハンドルの読み込み
	note_pageGHandle = LoadGraph("graph\\note_page.png");
	page1_turnoverGHandle = LoadGraph("movie\\1page_turnover.ogv");
	pagemany_turnoverGHandle = LoadGraph("movie\\manypages_turnover.ogv");
	reverse_page1_turnoverGHandle = LoadGraph("movie\\reverse_1page_turnover.ogv");
	reverse_pagemany_turnoverGHandle = LoadGraph("movie\\reverse_manypages_turnover.ogv");
	makibaTH_S64_T7 = LoadFontDataToHandle("fonts\\makiba_font.dft");

	//ゲーム処理
	SceneManager scene(make_shared<TitleScene>());
	while (ProcessMessage() == 0) {
		scene.Update();
		scene.Draw();
	}

	//終了処理
	//グローバル変数ハンドルの削除
	DeleteGraph(note_pageGHandle);
	DeleteGraph(page1_turnoverGHandle);
	DeleteGraph(pagemany_turnoverGHandle);
	DeleteGraph(reverse_page1_turnoverGHandle);
	DeleteGraph(reverse_pagemany_turnoverGHandle);
	DeleteFontToHandle(makibaTH_S64_T7);
	DxLib_End();
	return 0;
}

void ReloadFunctionInGame() {
	ReloadFileGraphAll();						// ファイルから読み込んだ画像を復元する

	//グローバル変数ハンドル復元
	makibaTH_S64_T7 = LoadFontDataToHandle("fonts\\makiba_font.dft");
	//動画ハンドル完全復元
	if (GetMovieStateToGraph(page1_turnoverGHandle) == 0) {
		page1_turnoverGHandle = LoadGraph("movie\\1page_turnover.ogv");
	}
	if (GetMovieStateToGraph(pagemany_turnoverGHandle) == 0) {
		pagemany_turnoverGHandle = LoadGraph("movie\\manypages_turnover.ogv");
	}
	if (GetMovieStateToGraph(reverse_page1_turnoverGHandle) == 0) {
		reverse_page1_turnoverGHandle = LoadGraph("movie\\reverse_1page_turnover.ogv");
	}
	if (GetMovieStateToGraph(reverse_pagemany_turnoverGHandle) == 0) {
		reverse_pagemany_turnoverGHandle = LoadGraph("movie\\reverse_manypages_turnover.ogv");
	}

	//シーン毎に設定された復元関数を呼び出す
	if (Scene_pointer_for_Reload) {
		Scene_pointer_for_Reload->ReloadFunction();
	}
}
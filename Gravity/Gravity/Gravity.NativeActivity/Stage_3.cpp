#include "Stage_3.h"
#include "GameClearScene.h"

Stage_3::Stage_3():GameBaseScene() {
	air_resistance_coefficient = 0.01;
	player_num = 3;
	size_up_ball_num = 0;
	charged_ball_num = 70;
	player = new std::shared_ptr<Player>[player_num];
	size_up_ball = new std::shared_ptr<NonMovableBall>[size_up_ball_num];
	charged_ball = new std::shared_ptr<MovableChargedBall>[charged_ball_num];
	player[0] = std::make_shared<Player>(240, 1280, -50, 8, 5, &accel_arrowGHandle, &playerGHandle);			//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル, &V矢印画像ハンドル)//初期座標は別ファイルから読み取るのがいいかもしれない
	player[1] = std::make_shared<Player>(540, 1280, -100, 12, 5, &accel_arrowGHandle, &playerGHandle);
	player[2] = std::make_shared<Player>(840, 1280, -50, 8, 5, &accel_arrowGHandle, &playerGHandle);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 7; j++) {
			charged_ball[10 * j + i] = std::make_shared<MovableChargedBall>(90 + 100 * i, 100 * (j + 1), 1, 2, 0.5, &charged_ballGHandle);	//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル)
		}
	}
	clear_count = 0;
	purpose = "小さな球を50個集める";
}

Stage_3::~Stage_3() {

}

//50個集めたらクリア
bool Stage_3::ClearChecker() {
	if (time_advances) {
		clear_count++;		//クリアフレーム確認用
	}
	bool check = false;
	int sum_check = 0;
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			sum_check += player[i]->Return_charge();
		}
	}
	if (sum_check >= -130) {
		check = true;
	}
	if (check) {
		return true;
	}
	else {
		return false;
	}
}

void Stage_3::GameClear() {
	if (clear_count / 60.0 <= 17) {
		star = 3;
	}
	else if (clear_count / 60.0 <= 20) {
		star = 2;
	}
	else if (clear_count / 60.0 <= 30) {
		star = 1;
	}
	else {
		star = 0;
	}
	phase = 1;
}
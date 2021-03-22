//テスト開発用
#include "Stage_Test.h"
#include "GameClearScene.h"

Stage_Test::Stage_Test() {
	air_resistance_coefficient = 0.01;
	player_num = 1;
	size_up_ball_num = 0;
	charged_ball_num = 3;
	player = new std::shared_ptr<Player>[player_num];
	size_up_ball = new std::shared_ptr<NonMovableBall>[size_up_ball_num];
	charged_ball = new std::shared_ptr<MovableChargedBall>[charged_ball_num];
	player[0] = std::make_shared<Player>(200, 500, 222, 5, 0.5, &charge_THandle, &accel_arrowGHandle, &playerGHandle);						//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル, &V矢印画像ハンドル)//初期座標は別ファイルから読み取るのがいいかもしれない
	charged_ball[0] = std::make_shared<MovableChargedBall>(750, 400, 0, 8, 0.5, &charge_THandle, &charged_ballGHandle);			//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル)
	charged_ball[1] = std::make_shared<MovableChargedBall>(100, 1100, 0, 8, 0.5, &charge_THandle, &charged_ballGHandle);
	charged_ball[2] = std::make_shared<MovableChargedBall>(250, 1700, 0, 8, 0.5, &charge_THandle, &charged_ballGHandle);
}

Stage_Test::~Stage_Test() {

}

bool Stage_Test::ClearChecker() {
	bool check = false;
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			if (player[i]->Return_charge() <= -5) {
				check = true;
				break;
			}
		}
	}
	if (check) {
		return true;
	}
	else {
		return false;
	}
}

void Stage_Test::GameClear() {
	int star, num;
	num = 5;
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			num = player[i]->Return_shoot_num();
			break;
		}
	}
	if (num <= 1) {
		star = 3;
	}
	else if (num <= 2) {
		star = 2;
	}
	else if (num <= 4) {
		star = 1;
	}
	else {
		star = 0;
	}
	nextScene = std::make_shared<GameClearScene>(star);
}
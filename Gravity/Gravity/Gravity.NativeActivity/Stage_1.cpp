#include "Stage_1.h"
#include "GameClearScene.h"

Stage_1::Stage_1():GameBaseScene() {
	air_resistance_coefficient = 0.01;
	player_num = 1;
	size_up_ball_num = 0;
	charged_ball_num = 3;
	player = new std::shared_ptr<Player>[player_num];
	size_up_ball = new std::shared_ptr<NonMovableBall>[size_up_ball_num];
	charged_ball = new std::shared_ptr<MovableChargedBall>[charged_ball_num];
	player[0] = std::make_shared<Player>(540, 1280, 5, 8, 0.5, &accel_arrowGHandle, &playerGHandle);						//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル, &V矢印画像ハンドル)//初期座標は別ファイルから読み取るのがいいかもしれない
	charged_ball[0] = std::make_shared<MovableChargedBall>(540, 200, -10, 8, 0.5, &charged_ballGHandle);			//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル)
	charged_ball[1] = std::make_shared<MovableChargedBall>(270, 740, 5, 8, 0.5, &charged_ballGHandle);
	charged_ball[2] = std::make_shared<MovableChargedBall>(810, 740, 5, 8, 0.5, &charged_ballGHandle);
	purpose = "球(-10)を取得する";
}

Stage_1::~Stage_1() {

}

bool Stage_1::ClearChecker() {
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

void Stage_1::GameClear() {
	int num;
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
	phase = 1;
}
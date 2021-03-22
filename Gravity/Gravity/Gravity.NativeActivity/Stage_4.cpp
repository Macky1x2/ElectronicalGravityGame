﻿#include "Stage_4.h"
#include "GameClearScene.h"

Stage_4::Stage_4() {
	air_resistance_coefficient = 0.01;
	player_num = 1;
	size_up_ball_num = 0;
	charged_ball_num = 37;
	player = new std::shared_ptr<Player>[player_num];
	size_up_ball = new std::shared_ptr<NonMovableBall>[size_up_ball_num];
	charged_ball = new std::shared_ptr<MovableChargedBall>[charged_ball_num];
	player[0] = std::make_shared<Player>(540, 1260, -5, 8, 50, &charge_THandle, &accel_arrowGHandle, &playerGHandle);			//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル, &V矢印画像ハンドル)//初期座標は別ファイルから読み取るのがいいかもしれない
	charged_ball[0] = std::make_shared<MovableChargedBall>(540, 960, -9999, 100, 100000000000, &charge_THandle, &charged_ballGHandle);			//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル)
	int r = 500;
	for (int i = 0; i < 36; i++) {
		charged_ball[i + 1] = std::make_shared<MovableChargedBall>(540 + r * cos((PI / 18) * i), 960 + r * sin((PI / 18) * i), -2000, 2, 1000000000000, &charge_THandle, &charged_ballGHandle);
	}
	purpose = "球(-9999)を取得する";
}

Stage_4::~Stage_4() {

}

bool Stage_4::ClearChecker() {
	bool check = false;
	int sum_check = 0;
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			sum_check += player[i]->Return_charge();
		}
	}
	if (sum_check <= -10000) {
		check = true;
	}
	if (check) {
		return true;
	}
	else {
		return false;
	}
}

void Stage_4::GameClear() {
	int star, num;
	num = 5;
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			num = player[i]->Return_shoot_num();
			break;
		}
	}
	if (num <= 3) {
		star = 3;
	}
	else if (num <= 5) {
		star = 2;
	}
	else if (num <= 10) {
		star = 1;
	}
	else {
		star = 0;
	}
	nextScene = std::make_shared<GameClearScene>(star);
}
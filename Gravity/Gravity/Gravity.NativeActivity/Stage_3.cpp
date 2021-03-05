#include "Stage_3.h"

Stage_3::Stage_3() {
	air_resistance_coefficient = 0.01;
	player_num = 3;
	size_up_ball_num = 0;
	charged_ball_num = 50;
	player = new std::shared_ptr<Player>[player_num];
	size_up_ball = new std::shared_ptr<NonMovableBall>[size_up_ball_num];
	charged_ball = new std::shared_ptr<MovableChargedBall>[charged_ball_num];
	player[0] = std::make_shared<Player>(540, 1280, -50, 8, 0.5, &charge_THandle, &accel_arrowGHandle);						//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル, &V矢印画像ハンドル)//初期座標は別ファイルから読み取るのがいいかもしれない
	player[1] = std::make_shared<Player>(340, 1280, -50, 8, 0.5, &charge_THandle, &accel_arrowGHandle);
	player[2] = std::make_shared<Player>(740, 1280, -50, 8, 0.5, &charge_THandle, &accel_arrowGHandle);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			charged_ball[10 * j + i] = std::make_shared<MovableChargedBall>(90 + 100 * i, 100 * (j + 1), 1, 2, 0.5, &charge_THandle);	//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル)
		}
	}
}

Stage_3::~Stage_3() {

}

bool Stage_3::ClearChecker() {
	bool check = false;
	int sum_check = 0;
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			sum_check += player[i]->Return_charge();
		}
	}
	if (sum_check >= -100) {
		check = true;
	}
	if (check) {
		return true;
	}
	else {
		return false;
	}
}
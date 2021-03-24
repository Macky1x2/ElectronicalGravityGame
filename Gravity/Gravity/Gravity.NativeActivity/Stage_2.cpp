#include "Stage_2.h"
#include "GameClearScene.h"

Stage_2::Stage_2() :GameBaseScene() {
	air_resistance_coefficient = 0.00;
	player_num = 1;
	size_up_ball_num = 0;
	charged_ball_num = 1;
	player = new std::shared_ptr<Player>[player_num];
	size_up_ball = new std::shared_ptr<NonMovableBall>[size_up_ball_num];
	charged_ball = new std::shared_ptr<MovableChargedBall>[charged_ball_num];
	player[0] = std::make_shared<Player>(540, 1280, 5, 8, 0.5, &accel_arrowGHandle, &playerGHandle);						//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル, &V矢印画像ハンドル)//初期座標は別ファイルから読み取るのがいいかもしれない
	charged_ball[0] = std::make_shared<MovableChargedBall>(540, 740, -300, 100, 1000000, &charged_ballGHandle);			//引数(初期x座標, 初期y座標, 電荷, 体積, 密度, &テキストハンドル)
	check_radian = 0;
	pre_radian = PI / 2;
	purpose = "大きな球の周りを3週回る";
}

Stage_2::~Stage_2() {

}

//3回転したらクリア
bool Stage_2::ClearChecker() {
	if (player[0] && charged_ball[0]) {
		double now_radian = atan2(player[0]->Return_position_y() - charged_ball[0]->Return_position_y(), player[0]->Return_position_x() - charged_ball[0]->Return_position_x());
		if (pre_radian<-(3.0 / 4) * PI && now_radian>(3.0 / 4) * PI) {
			check_radian += ((now_radian - 2 * PI) - pre_radian);
		}
		else if (pre_radian > (3.0 / 4) * PI && now_radian < -(3.0 / 4) * PI) {
			check_radian += ((now_radian + 2 * PI) - pre_radian);
		}
		else {
			check_radian += (now_radian - pre_radian);
		}
		if (check_radian >= 2 * PI * 3 || check_radian <= -2 * PI * 3) {
			return true;
		}
		pre_radian = now_radian;
	}
	return false;
}

void Stage_2::GameClear() {
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
	else if (num <= 3) {
		star = 1;
	}
	else {
		star = 0;
	}
	phase = 1;
}
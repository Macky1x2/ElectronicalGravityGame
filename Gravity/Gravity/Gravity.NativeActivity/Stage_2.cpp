#include "Stage_2.h"

Stage_2::Stage_2() {
	air_resistance_coefficient = 0.00;
	player_num = 1;
	size_up_ball_num = 0;
	charged_ball_num = 1;
	player = new std::shared_ptr<Player>[player_num];
	size_up_ball = new std::shared_ptr<NonMovableBall>[size_up_ball_num];
	charged_ball = new std::shared_ptr<MovableChargedBall>[charged_ball_num];
	player[0] = std::make_shared<Player>(540, 1280, 5, 8, 0.5, &charge_THandle, &accel_arrowGHandle);						//����(����x���W, ����y���W, �d��, �̐�, ���x, &�e�L�X�g�n���h��, &V���摜�n���h��)//�������W�͕ʃt�@�C������ǂݎ��̂�������������Ȃ�
	charged_ball[0] = std::make_shared<MovableChargedBall>(540, 740, -300, 100, 1000000, &charge_THandle);			//����(����x���W, ����y���W, �d��, �̐�, ���x, &�e�L�X�g�n���h��)
	check_radian = 0;
	pre_radian = PI / 2;
}

Stage_2::~Stage_2() {

}

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
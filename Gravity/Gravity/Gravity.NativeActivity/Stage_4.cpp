#include "Stage_4.h"

Stage_4::Stage_4() {
	air_resistance_coefficient = 0.01;
	player_num = 1;
	size_up_ball_num = 0;
	charged_ball_num = 37;
	player = new std::shared_ptr<Player>[player_num];
	size_up_ball = new std::shared_ptr<NonMovableBall>[size_up_ball_num];
	charged_ball = new std::shared_ptr<MovableChargedBall>[charged_ball_num];
	player[0] = std::make_shared<Player>(540, 1260, -5, 8, 50, &charge_THandle, &accel_arrowGHandle);			//����(����x���W, ����y���W, �d��, �̐�, ���x, &�e�L�X�g�n���h��, &V���摜�n���h��)//�������W�͕ʃt�@�C������ǂݎ��̂�������������Ȃ�
	charged_ball[0] = std::make_shared<MovableChargedBall>(540, 960, -9999, 100, 100000000000, &charge_THandle);			//����(����x���W, ����y���W, �d��, �̐�, ���x, &�e�L�X�g�n���h��)
	int r = 500;
	for (int i = 0; i < 36; i++) {
		charged_ball[i + 1] = std::make_shared<MovableChargedBall>(540 + r * cos((PI / 18) * i), 960 + r * sin((PI / 18) * i), -2000, 2, 1000000000000, &charge_THandle);
	}
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
#include "TestGameScene.h"

TestGameScene::TestGameScene() {
	player = std::make_shared<Player>();
	size_up_ball[0] = std::make_shared<NonMovableBall>(300, 1000);		//�������W�͕ʃt�@�C������ǂݎ��̂�������������Ȃ�
	size_up_ball[1] = std::make_shared<NonMovableBall>(600, 1500);
	size_up_ball[2] = std::make_shared<NonMovableBall>(1050, 300);
	charged_ball[0] = std::make_shared<MovableChargedBall>(750, 400);
	charged_ball[1] = std::make_shared<MovableChargedBall>(100, 1100);
	charged_ball[2] = std::make_shared<MovableChargedBall>(250, 1700);
}

TestGameScene::~TestGameScene() {

}

void TestGameScene::HitConbine() {
	//�v���C���[�Ɠ����Ȃ��{�[���Ԃɂ���
	for (int i = 0; i < std::extent<decltype(size_up_ball), 0>::value; i++) {
		if (player && size_up_ball[i]) {
			if (HitChecker_PlayerandNonMovableBall(player, size_up_ball[i])) {
				//�^���ʕۑ���
				double m = player->Return_density() * player->Return_volume() + size_up_ball[i]->Return_density() * size_up_ball[i]->Return_volume();
				player->Decide_speed_x(player->Return_density() * player->Return_volume() * player->Return_speed_x() / m);
				player->Decide_speed_y(player->Return_density() * player->Return_volume() * player->Return_speed_y() / m);
				
				//�v���C���[���ɉ��Z
				player->Add_volume(size_up_ball[i]->Return_volume());
				size_up_ball[i].reset();
			}
		}
	}

	//�v���C���[�Ɖ��ȓd�C��тт��{�[���Ԃɂ���
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (player && charged_ball[i]) {
			if (HitChecker_PlayerandMovableChargedBall(player, charged_ball[i])) {
				//�^���ʕۑ���
				double m = player->Return_density() * player->Return_volume() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume();
				player->Decide_speed_x((player->Return_density() * player->Return_volume() * player->Return_speed_x() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_x()) / m);
				player->Decide_speed_y((player->Return_density() * player->Return_volume() * player->Return_speed_y() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_y()) / m);

				//�v���C���[���ɉ��Z
				player->Add_volume(charged_ball[i]->Return_volume());
				player->Add_charge(charged_ball[i]->Return_charge());
				player->Make_TGHandle();
				charged_ball[i].reset();
			}
		}
	}

	//���ȓd�C��тт��{�[���Ɠ����Ȃ��{�[���Ԃɂ���
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		for (int j = 0; j < std::extent<decltype(size_up_ball), 0>::value; j++) {
			if (charged_ball[i] && size_up_ball[j]) {
				if (HitChecker_MovableChargedBallandNonMovableBall(charged_ball[i], size_up_ball[j])) {
					//�^���ʕۑ���
					double m = charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() + size_up_ball[j]->Return_density() * size_up_ball[j]->Return_volume();
					charged_ball[i]->Decide_speed_x(charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_x() / m);
					charged_ball[i]->Decide_speed_y(charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_y() / m);

					//���ȓd�C��тт��{�[�����ɉ��Z
					charged_ball[i]->Add_volume(size_up_ball[j]->Return_volume());
					size_up_ball[j].reset();
				}
			}
		}
	}
}

void TestGameScene::Gravity() {
	//������
	if (player) {
		player->Decide_force_x(0.0);
		player->Decide_force_y(0.0);
	}
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Decide_force_x(0.0);
			charged_ball[i]->Decide_force_y(0.0);
		}
	}

	//�v���C���[�E�d�ׂ�тт����ȃ{�[���Ԃ̈���
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (player && charged_ball[i]) {
			double r = pow((charged_ball[i]->Return_position_x() - player->Return_position_x()) * (charged_ball[i]->Return_position_x() - player->Return_position_x()) + (charged_ball[i]->Return_position_y() - player->Return_position_y()) * (charged_ball[i]->Return_position_y() - player->Return_position_y()), 1.0 / 2);
			double temp_x, temp_y;
			temp_x = -COULOMB_CONSTANT * player->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_x() - player->Return_position_x()) / (r * r * r);
			temp_y = -COULOMB_CONSTANT * player->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_y() - player->Return_position_y()) / (r * r * r);
			player->Add_force_x(temp_x);
			player->Add_force_y(temp_y);
			charged_ball[i]->Add_force_x(-temp_x);
			charged_ball[i]->Add_force_y(-temp_y);
		}
	}

	//�d�ׂ�тт����ȃ{�[�����m�̈���
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		for (int j = i + 1; j < std::extent<decltype(charged_ball), 0>::value; j++) {
			if (charged_ball[i] && charged_ball[j]) {
				double r = pow((charged_ball[i]->Return_position_x() - charged_ball[j]->Return_position_x()) * (charged_ball[i]->Return_position_x() - charged_ball[j]->Return_position_x()) + (charged_ball[i]->Return_position_y() - charged_ball[j]->Return_position_y()) * (charged_ball[i]->Return_position_y() - charged_ball[j]->Return_position_y()), 1.0 / 2);
				double temp_x, temp_y;
				temp_x = -COULOMB_CONSTANT * charged_ball[j]->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_x() - charged_ball[j]->Return_position_x()) / (r * r * r);
				temp_y = -COULOMB_CONSTANT * charged_ball[j]->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_y() - charged_ball[j]->Return_position_y()) / (r * r * r);
				charged_ball[j]->Add_force_x(temp_x);
				charged_ball[j]->Add_force_y(temp_y);
				charged_ball[i]->Add_force_x(-temp_x);
				charged_ball[i]->Add_force_y(-temp_y);
			}
		}
	}
}

void TestGameScene::Update() {
	Gravity();					//���͏���
	if (player) {
		player->Update();
	}
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Update();
		}
	}
	HitConbine();				//�Փ�then��������
}

void TestGameScene::Draw()const {
	for (int i = 0; i < std::extent<decltype(size_up_ball), 0>::value; i++) {
		if (size_up_ball[i]) {
			size_up_ball[i]->Draw();
		}
	}
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Draw();
		}
	}
	if (player) {
		player->Draw();
	}
}

bool TestGameScene::HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player> _player, std::shared_ptr<NonMovableBall> _size_up_ball) {
	//if(�������Ă���Ȃ��)
	if ((_player->Return_position_x() - _size_up_ball->Return_position_x()) * (_player->Return_position_x() - _size_up_ball->Return_position_x()) + (_player->Return_position_y() - _size_up_ball->Return_position_y()) * (_player->Return_position_y() - _size_up_ball->Return_position_y()) < (_player->Return_radius() + _size_up_ball->Return_radius()) * (_player->Return_radius() + _size_up_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool TestGameScene::HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player> _player, std::shared_ptr<MovableChargedBall> _charged_ball) {
	//if(�������Ă���Ȃ��)
	if ((_player->Return_position_x() - _charged_ball->Return_position_x()) * (_player->Return_position_x() - _charged_ball->Return_position_x()) + (_player->Return_position_y() - _charged_ball->Return_position_y()) * (_player->Return_position_y() - _charged_ball->Return_position_y()) < (_player->Return_radius() + _charged_ball->Return_radius()) * (_player->Return_radius() + _charged_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool TestGameScene::HitChecker_MovableChargedBallandNonMovableBall(std::shared_ptr<MovableChargedBall> _charged_ball, std::shared_ptr<NonMovableBall> _size_up_ball) {
	//if(�������Ă���Ȃ��)
	if ((_charged_ball->Return_position_x() - _size_up_ball->Return_position_x()) * (_charged_ball->Return_position_x() - _size_up_ball->Return_position_x()) + (_charged_ball->Return_position_y() - _size_up_ball->Return_position_y()) * (_charged_ball->Return_position_y() - _size_up_ball->Return_position_y()) < (_charged_ball->Return_radius() + _size_up_ball->Return_radius()) * (_charged_ball->Return_radius() + _size_up_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}
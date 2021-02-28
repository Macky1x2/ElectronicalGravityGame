#include "Actor.h"
#include <cmath>
#include <math.h>

Player::Player() {
	volume = 5;
	radius = 50;
	own_color = GetColor(0, 255, 255);
	volume_text_color = GetColor(255, 0, 0);
	position_x = 500;
	position_y = 500;
	tap_checker_pre = false;
	tap_checker_now = false;
	speed_x = 0;
	speed_y = 0;
	volume_THandle= CreateFontToHandle(NULL, 20, 6, DX_FONTTYPE_NORMAL);					//���ߕK�{:�t�H���g�ɓK�����t�H���g�T�C�Y
	volume_text_width = GetDrawFormatStringWidthToHandle(volume_THandle, "%d", volume);
	volume_temp_GHandle = MakeScreen(volume_text_width, 20, TRUE);							//���ߕK�{:��,����
	SetDrawScreen(volume_temp_GHandle);
	DrawFormatStringToHandle(0, -3, volume_text_color, volume_THandle, "%d", volume);		//���ߕK�{:�N�_y���W
	SetDrawScreen(DX_SCREEN_BACK);
	accel_arrowGHandle = LoadGraph("V_arrow_red.png");
	accel_arrow_num = 0;
	charge = 5;
	acceleration_x = 0;
	acceleration_y = 0;
	force_x = 0;
	force_y = 0;
	density = 0.5;
}

Player::~Player() {
	
}

void Player::Update() {
	//�͌��菈�����I����
	acceleration_x = force_x / (density * volume);
	acceleration_y = force_y / (density * volume);
	//�����x���菈�����I����
	speed_x += acceleration_x;
	speed_y += acceleration_y;

	if (GetTouchInputNum() == 1) {
		tap_checker_now = true;
	}

	//�ړ�����
	if (tap_checker_now == true && tap_checker_pre == false) {
		GetTouchInput(0, &accel_start_x, &accel_start_y, NULL, NULL);
		accel_temp_x = accel_start_x;
		accel_temp_y = accel_start_y;
	}
	else if (tap_checker_now == false && tap_checker_pre == true) {
		accel_end_x = accel_temp_x;
		accel_end_y = accel_temp_y;
		accel_vector_size = std::sqrt(((accel_start_x - accel_end_x) * (accel_start_x - accel_end_x) + (accel_start_y - accel_end_y) * (accel_start_y - accel_end_y))*1.0);
		accel_power = accel_vector_size / 50;
		accel_arrow_num = 0;
		if (accel_vector_size != 0) {
			speed_x += accel_power * ((accel_start_x - accel_end_x) / accel_vector_size);
			speed_y += accel_power * ((accel_start_y - accel_end_y) / accel_vector_size);
		}
	}
	else if(tap_checker_now == true && tap_checker_pre == true){
		GetTouchInput(0, &accel_temp_x, &accel_temp_y, NULL, NULL);
		if (!(accel_start_x - accel_temp_x == 0 && accel_start_y - accel_temp_y == 0)) {
			accel_arrow_direction = std::atan2(accel_start_y - accel_temp_y, accel_start_x - accel_temp_x);
			accel_arrow_num = std::sqrt(((accel_start_x - accel_temp_x) * (accel_start_x - accel_temp_x) + (accel_start_y - accel_temp_y) * (accel_start_y - accel_temp_y)) * 1.0) / 200;
		}
		else {
			accel_arrow_num = 0;
		}
	}

	//���x���菈�����I����
	position_x += speed_x;
	position_y += speed_y;
	//�̐ό��菈�����I����
	volume_text_width = GetDrawFormatStringWidthToHandle(volume_THandle, "%d", volume);
	volume_temp_GHandle = MakeScreen(volume_text_width, 20, TRUE);
	SetDrawScreen(volume_temp_GHandle);
	DrawFormatStringToHandle(0, -3, volume_text_color, volume_THandle, "%d", volume);
	SetDrawScreen(DX_SCREEN_BACK);
	//tap���菈�����I����
	if (tap_checker_now == true) {
		tap_checker_now = false;
		tap_checker_pre = true;
	}
	else if (tap_checker_now == false) {
		tap_checker_pre = false;
	}
}

void Player::Draw()const {
	DrawCircle(position_x, position_y, radius, own_color, TRUE);							//�����`��
	DrawRotaGraph(position_x, position_y, 3.0, 0.0, volume_temp_GHandle, TRUE, FALSE);		//�̐ϕ����`��
	//�������`��
	for (int i = 0; i < accel_arrow_num; i++) {
		DrawRotaGraph(position_x - (i + 1) * (radius * 1.5) * cos(accel_arrow_direction), position_y - (i + 1) * (radius * 1.5) * sin(accel_arrow_direction), 0.3, accel_arrow_direction, accel_arrowGHandle, TRUE, FALSE);
	}
}

int Player::Return_volume() {
	return volume;
}

int Player::Return_charge() {
	return charge;
}

double Player::Return_position_x() {
	return position_x;
}

double Player::Return_position_y() {
	return position_y;
}

double Player::Return_radius() {
	return radius;
}

double Player::Return_force_x() {
	return force_x;
}

double Player::Return_force_y() {
	return force_y;
}

void Player::Decide_force_x(double decide_x) {
	force_x = decide_x;
}

void Player::Decide_force_y(double decide_y) {
	force_y = decide_y;
}

void Player::Change_radiusbyvolume(int _volume) {
	radius = 50 * pow(_volume / 5.0, 1.0 / 3);
}

void Player::Add_volume(int add_volume) {
	volume += add_volume;
	Change_radiusbyvolume(volume);			//�̐ς��ω��������ߔ��a���ω�������
}

NonMovableBall::NonMovableBall(double first_x, double first_y) {
	volume = 3;
	radius = 50 * pow(volume / 5.0, 1.0 / 3);
	own_color = GetColor(255, 255, 255);
	volume_text_color = GetColor(255, 0, 0);
	position_x = first_x;
	position_y = first_y;
	density = 0.5;
}

NonMovableBall::~NonMovableBall() {

}

void NonMovableBall::Update() {

}

void NonMovableBall::Draw()const {
	DrawCircle(position_x, position_y, radius, own_color, TRUE);
}

int NonMovableBall::Return_volume() {
	return volume;
}

double NonMovableBall::Return_position_x() {
	return position_x;
}

double NonMovableBall::Return_position_y() {
	return position_y;
}

double NonMovableBall::Return_radius() {
	return radius;
}

MovableChargedBall::MovableChargedBall(double first_x, double first_y) :NonMovableBall(first_x, first_y) {
	charge = -5;
	speed_x = 0;
	speed_y = 0;
	acceleration_x = 0;
	acceleration_y = 0;
	force_x = 0;
	force_y = 0;
	own_color = GetColor(0, 255, 0);
}

MovableChargedBall::~MovableChargedBall() {

}

void MovableChargedBall::Update() {
	//�͌��菈�����I����
	acceleration_x = force_x / (density * volume);
	acceleration_y = force_y / (density * volume);
	//�����x���菈�����I����
	speed_x += acceleration_x;
	speed_y += acceleration_y;
	//���x���菈�����I����
	position_x += speed_x;
	position_y += speed_y;
}

int MovableChargedBall::Return_charge() {
	return charge;
}

void MovableChargedBall::Decide_force_x(double decide_x) {
	force_x = decide_x;
}

void MovableChargedBall::Decide_force_y(double decide_y) {
	force_y = decide_y;
}
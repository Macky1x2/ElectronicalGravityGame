#include "Actor.h"

#define MAKIBA_Y_CORRECTION -2

extern int makibaTH_S64_T7;

Player::Player(double first_x, double first_y, int _charge, int _volume, double _density, int* _accel_arrowGHandle, int* _ownGHandle) {
	volume = _volume;
	radius = 50 * pow(volume / 5.0, 1.0 / 3);
	own_color = GetColor(0, 255, 255);
	charge_text_color = GetColor(255, 0, 0);
	position_x = first_x;
	position_y = first_y;
	tap_checker_pre = false;
	tap_checker_now = false;
	checker_when_time_stopped = false;
	speed_x = 0;
	speed_y = 0;
	charge = _charge;
	if (charge > 0) {
		charge_text_width = GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "+%d", charge);
	}
	else {
		charge_text_width = GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "%d", charge);
	}
	charge_temp_GHandle = MakeScreen(charge_text_width, 64, TRUE);							//調節必須:幅,高さ
	SetDrawScreen(charge_temp_GHandle);
	if (charge > 0) {
		DrawFormatStringToHandle(0, MAKIBA_Y_CORRECTION, charge_text_color, makibaTH_S64_T7, "+%d", charge);	//調節必須:起点y座標
	}
	else {
		DrawFormatStringToHandle(0, MAKIBA_Y_CORRECTION, charge_text_color, makibaTH_S64_T7, "%d", charge);
	}
	SetDrawScreen(DX_SCREEN_BACK);
	accel_arrowGHandle = _accel_arrowGHandle;
	ownGHandle = _ownGHandle;
	accel_arrow_num = 0;
	acceleration_x = 0;
	acceleration_y = 0;
	force_x = 0;
	force_y = 0;
	density = _density;
	shoot_num = 0;
}

Player::~Player() {
	DeleteGraph(charge_temp_GHandle);
}

void Player::Update() {
	//力決定処理が終了後
	acceleration_x = force_x / (density * volume);
	acceleration_y = force_y / (density * volume);
	//加速度決定処理が終了後
	speed_x += acceleration_x;
	speed_y += acceleration_y;

	Shoot_Operation();								//移動操作

	//速度決定処理が終了後
	position_x += speed_x;
	position_y += speed_y;
	//体積決定処理が終了後
}

void Player::Draw()const {
	DrawRotaGraph(position_x, position_y, 0.00185*radius, 0, *ownGHandle, TRUE, FALSE);		//自分描画

	//体積文字描画
	if (charge_text_width > 55) {
		DrawRotaGraph(position_x, position_y, (55.0 / charge_text_width) * 1.5 * radius / 50, 0.0, charge_temp_GHandle, TRUE, FALSE);
	}
	else {
		DrawRotaGraph(position_x, position_y, 1.5 * radius / 50, 0.0, charge_temp_GHandle, TRUE, FALSE);
	}

	//加速矢印描画
	for (int i = 0; i < accel_arrow_num; i++) {
		DrawRotaGraph(position_x - (i + 1) * (radius * 1.5) * cos(accel_arrow_direction), position_y - (i + 1) * (radius * 1.5) * sin(accel_arrow_direction), 0.3, accel_arrow_direction, *accel_arrowGHandle, TRUE, FALSE);
	}
}

int Player::Return_volume() {
	return volume;
}

int Player::Return_charge() {
	return charge;
}

int Player::Return_shoot_num() {
	return shoot_num;
}

double Player::Return_position_x() {
	return position_x;
}

double Player::Return_position_y() {
	return position_y;
}

double Player::Return_speed_x() {
	return speed_x;
}

double Player::Return_speed_y() {
	return speed_y;
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

double Player::Return_density() {
	return density;
}

bool Player::Return_checker_when_time_stopped() {
	return checker_when_time_stopped;
}

void Player::Decide_position_x(double decide_x) {
	position_x = decide_x;
}

void Player::Decide_position_y(double decide_y) {
	position_y = decide_y;
}

void Player::Decide_force_x(double decide_x) {
	force_x = decide_x;
}

void Player::Decide_force_y(double decide_y) {
	force_y = decide_y;
}

void Player::Decide_speed_x(double decide_x) {
	speed_x = decide_x;
}

void Player::Decide_speed_y(double decide_y) {
	speed_y = decide_y;
}

void Player::Decide_density(double decide_den) {
	density = decide_den;
}

void Player::Change_radiusbyvolume(int _volume) {
	radius = 50 * pow(_volume / 5.0, 1.0 / 3);
}

void Player::Add_volume(int add_volume) {
	volume += add_volume;
	Change_radiusbyvolume(volume);			//体積が変化したため半径も変化させる
}

void Player::Add_charge(int add_charge) {
	charge += add_charge;
}

void Player::Add_force_x(double add_force) {
	force_x += add_force;
}

void Player::Add_force_y(double add_force) {
	force_y += add_force;
}

void Player::Make_TGHandle() {
	//正ならば符号は+, 負ならば符号は-, 0ならば符号なし
	if (charge > 0) {
		charge_text_width = GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "+%d", charge);
	}
	else {
		charge_text_width = GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "%d", charge);
	}
	//復元時データが破損している恐れがあるため削除
	DeleteGraph(charge_temp_GHandle);
	charge_temp_GHandle = MakeScreen(charge_text_width, 64, TRUE);
	//電荷描画用画像生成
	SetDrawScreen(charge_temp_GHandle);
	if (charge > 0) {
		DrawFormatStringToHandle(0, MAKIBA_Y_CORRECTION, charge_text_color, makibaTH_S64_T7, "+%d", charge);
	}
	else {
		DrawFormatStringToHandle(0, MAKIBA_Y_CORRECTION, charge_text_color, makibaTH_S64_T7, "%d", charge);
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

//プレイヤー操作
void Player::Shoot_Operation() {
	checker_when_time_stopped = false;
	if (GetTouchInputNum() == 1) {
		tap_checker_now = true;
	}
	if (tap_checker_now == true && tap_checker_pre == false) {
		GetTouchInput(0, &accel_start_x, &accel_start_y, NULL, NULL);
		accel_temp_x = accel_start_x;
		accel_temp_y = accel_start_y;
	}
	else if (tap_checker_now == false && tap_checker_pre == true) {
		accel_end_x = accel_temp_x;
		accel_end_y = accel_temp_y;
		accel_vector_size = std::sqrt(((accel_start_x - accel_end_x) * (accel_start_x - accel_end_x) + (accel_start_y - accel_end_y) * (accel_start_y - accel_end_y)) * 1.0);
		accel_power = accel_vector_size / 50;
		accel_arrow_num = 0;
		if (accel_vector_size != 0) {
			speed_x += (2.5 / sqrt(density * volume)) * accel_power * ((accel_start_x - accel_end_x) / accel_vector_size);
			speed_y += (2.5 / sqrt(density * volume)) * accel_power * ((accel_start_y - accel_end_y) / accel_vector_size);
			checker_when_time_stopped = true;
			shoot_num++;
		}
	}
	else if (tap_checker_now == true && tap_checker_pre == true) {
		GetTouchInput(0, &accel_temp_x, &accel_temp_y, NULL, NULL);
		if (!(accel_start_x - accel_temp_x == 0 && accel_start_y - accel_temp_y == 0)) {
			accel_arrow_direction = std::atan2(accel_start_y - accel_temp_y, accel_start_x - accel_temp_x);
			accel_arrow_num = std::sqrt(((accel_start_x - accel_temp_x) * (accel_start_x - accel_temp_x) + (accel_start_y - accel_temp_y) * (accel_start_y - accel_temp_y)) * 1.0) / 200;
		}
		else {
			accel_arrow_num = 0;
		}
	}
	if (tap_checker_now == true) {
		tap_checker_now = false;
		tap_checker_pre = true;
	}
	else if (tap_checker_now == false) {
		tap_checker_pre = false;
	}
}

NonMovableBall::NonMovableBall(double first_x, double first_y, int _volume, double _density, int* _ownGHandle) {
	volume = _volume;
	radius = 50 * pow(volume / 5.0, 1.0 / 3);
	own_color = GetColor(255, 255, 255);
	position_x = first_x;
	position_y = first_y;
	density = _density;
	ownGHandle = _ownGHandle;
}

NonMovableBall::~NonMovableBall() {

}

void NonMovableBall::Update() {

}

void NonMovableBall::Draw()const {
	DrawRotaGraph(position_x, position_y, 0.00185 * radius, 0, *ownGHandle, TRUE, FALSE);		//自分描画
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

double NonMovableBall::Return_density() {
	return density;
}

MovableChargedBall::MovableChargedBall(double first_x, double first_y, int _charge, int _volume, double _density, int* _ownGHandle) :NonMovableBall(first_x, first_y, _volume, _density, _ownGHandle) {
	charge = _charge;
	speed_x = 0;
	speed_y = 0;
	acceleration_x = 0;
	acceleration_y = 0;
	force_x = 0;
	force_y = 0;
	own_color = GetColor(0, 255, 0);
	charge_text_color = GetColor(0, 0, 0);
	if (charge > 0) {
		charge_text_width = GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "+%d", charge);
	}
	else {
		charge_text_width = GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "%d", charge);
	}
	charge_temp_GHandle = MakeScreen(charge_text_width, 64, TRUE);							//調節必須:幅,高さ
	SetDrawScreen(charge_temp_GHandle);
	if (charge > 0) {
		DrawFormatStringToHandle(0, MAKIBA_Y_CORRECTION, charge_text_color, makibaTH_S64_T7, "+%d", charge);	//調節必須:起点y座標
	}
	else {
		DrawFormatStringToHandle(0, MAKIBA_Y_CORRECTION, charge_text_color, makibaTH_S64_T7, "%d", charge);
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

MovableChargedBall::~MovableChargedBall() {
	DeleteGraph(charge_temp_GHandle);
}

void MovableChargedBall::Update() {
	//力決定処理が終了後
	acceleration_x = force_x / (density * volume);
	acceleration_y = force_y / (density * volume);
	//加速度決定処理が終了後
	speed_x += acceleration_x;
	speed_y += acceleration_y;
	//速度決定処理が終了後
	position_x += speed_x;
	position_y += speed_y;
	//体積決定処理が終了後
}

void MovableChargedBall::Draw()const {
	DrawRotaGraph(position_x, position_y, 0.00185 * radius, 0, *ownGHandle, TRUE, FALSE);		//自分描画
	
	//体積文字描画
	if (charge_text_width > 55) {
		DrawRotaGraph(position_x, position_y, (55.0 / charge_text_width) * 1.5 * radius / 50, 0.0, charge_temp_GHandle, TRUE, FALSE);
	}
	else {
		DrawRotaGraph(position_x, position_y, 1.5 * radius / 50, 0.0, charge_temp_GHandle, TRUE, FALSE);
	}
}

int MovableChargedBall::Return_charge() {
	return charge;
}

double MovableChargedBall::Return_speed_x() {
	return speed_x;
}

double MovableChargedBall::Return_speed_y() {
	return speed_y;
}

double MovableChargedBall::Return_force_x() {
	return force_x;
}

double MovableChargedBall::Return_force_y() {
	return force_y;
}

void MovableChargedBall::Decide_position_x(double decide_x) {
	position_x = decide_x;
}

void MovableChargedBall::Decide_position_y(double decide_y) {
	position_y = decide_y;
}

void MovableChargedBall::Decide_force_x(double decide_x) {
	force_x = decide_x;
}

void MovableChargedBall::Decide_force_y(double decide_y) {
	force_y = decide_y;
}

void MovableChargedBall::Decide_speed_x(double decide_x) {
	speed_x = decide_x;
}

void MovableChargedBall::Decide_speed_y(double decide_y) {
	speed_y = decide_y;
}

void MovableChargedBall::Decide_density(double decide_den) {
	density = decide_den;
}

void MovableChargedBall::Add_force_x(double add_force) {
	force_x += add_force;
}

void MovableChargedBall::Add_force_y(double add_force) {
	force_y += add_force;
}

void MovableChargedBall::Add_volume(int add_volume) {
	volume += add_volume;
	Change_radiusbyvolume(volume);			//体積が変化したため半径も変化させる
}

void MovableChargedBall::Add_charge(int add_charge) {
	charge += add_charge;
}

void MovableChargedBall::Change_radiusbyvolume(int _volume) {
	radius = 50 * pow(_volume / 5.0, 1.0 / 3);
}

void MovableChargedBall::Make_TGHandle() {
	if (charge > 0) {
		charge_text_width = GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "+%d", charge);
	}
	else {
		charge_text_width = GetDrawFormatStringWidthToHandle(makibaTH_S64_T7, "%d", charge);
	}
	DeleteGraph(charge_temp_GHandle);
	charge_temp_GHandle = MakeScreen(charge_text_width, 64, TRUE);
	SetDrawScreen(charge_temp_GHandle);
	if (charge > 0) {
		DrawFormatStringToHandle(0, MAKIBA_Y_CORRECTION, charge_text_color, makibaTH_S64_T7, "+%d", charge);
	}
	else {
		DrawFormatStringToHandle(0, MAKIBA_Y_CORRECTION, charge_text_color, makibaTH_S64_T7, "%d", charge);
	}
	SetDrawScreen(DX_SCREEN_BACK);
}
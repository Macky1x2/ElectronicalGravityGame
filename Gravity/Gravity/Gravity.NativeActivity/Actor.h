#pragma once
#include "DxLib.h"
#include "define.h"
#include <cmath>

//操作するボール
class Player {
	bool tap_checker_pre, tap_checker_now;
	bool checker_when_time_stopped;
	int volume;
	unsigned int own_color, charge_text_color;
	double radius;
	double position_x, position_y;
	double speed_x, speed_y;
	int accel_start_x, accel_start_y, accel_end_x, accel_end_y, accel_temp_x, accel_temp_y;
	double accel_power, accel_vector_size, accel_arrow_direction;
	int accel_arrowGHandle, accel_arrow_num;
	int charge, charge_THandle, charge_temp_GHandle, charge_text_width;
	int shoot_num;
	double acceleration_x, acceleration_y;
	double force_x, force_y;
	double density;
public:
	Player(double, double, int, int, double);		//引数(初期x座標, 初期y座標, 電荷, 体積, 密度)
	~Player();
	void Update();
	void Draw()const;
	int Return_volume();
	int Return_charge();
	int Return_shoot_num();
	double Return_position_x();
	double Return_position_y();
	double Return_speed_x();
	double Return_speed_y();
	double Return_radius();
	double Return_force_x();
	double Return_force_y();
	double Return_density();
	bool Return_checker_when_time_stopped();
	void Add_volume(int);
	void Add_charge(int);
	void Add_force_x(double);
	void Add_force_y(double);
	void Change_radiusbyvolume(int);
	void Decide_force_x(double);
	void Decide_force_y(double);
	void Decide_speed_x(double);
	void Decide_speed_y(double);
	void Decide_density(double);
	void Make_TGHandle();
	void Shoot_Operation();
	void Load_THandle();
};

//動かない引力なしのボール
class NonMovableBall {
protected:
	int volume;
	unsigned int own_color;
	double position_x, position_y;
	double radius;
	double density;
public:
	NonMovableBall(double, double, int, double);				//引数(初期x座標, 初期y座標, 体積, 密度)
	~NonMovableBall();
	void Update();
	void Draw()const;
	int Return_volume();
	double Return_position_x();
	double Return_position_y();
	double Return_radius();
	double Return_density();
};

//可動な、電気を帯びたボール(電荷0も含む)
class MovableChargedBall :public NonMovableBall {
	int charge, charge_THandle, charge_temp_GHandle, charge_text_width;
	unsigned int charge_text_color;
	double speed_x, speed_y;
	double acceleration_x, acceleration_y;
	double force_x, force_y;
public:
	MovableChargedBall(double, double, int, int, double);		//引数(初期x座標, 初期y座標, 電荷, 体積, 密度)
	~MovableChargedBall();
	void Update();
	void Draw()const;
	int Return_charge();
	double Return_speed_x();
	double Return_speed_y();
	double Return_force_x();
	double Return_force_y();
	void Decide_force_x(double);
	void Decide_force_y(double);
	void Decide_speed_x(double);
	void Decide_speed_y(double);
	void Decide_density(double);
	void Add_force_x(double);
	void Add_force_y(double);
	void Add_volume(int);
	void Add_charge(int);
	void Change_radiusbyvolume(int);
	void Make_TGHandle();
	void Load_THandle();
};
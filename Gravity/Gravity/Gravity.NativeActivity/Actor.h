#pragma once
#include "DxLib.h"
#include "define.h"

//操作するボール
class Player {
	bool tap_checker_pre, tap_checker_now;
	int volume, volume_THandle, volume_temp_GHandle, volume_text_width;
	unsigned int own_color, volume_text_color;
	double radius;
	double position_x, position_y;
	double speed_x, speed_y;
	int accel_start_x, accel_start_y, accel_end_x, accel_end_y, accel_temp_x, accel_temp_y;
	double accel_power, accel_vector_size, accel_arrow_direction;
	int accel_arrowGHandle, accel_arrow_num;
	int charge;
	double acceleration_x, acceleration_y;
	double force_x, force_y;
	double density;
public:
	Player();
	~Player();
	void Update();
	void Draw()const;
	int Return_volume();
	int Return_charge();
	double Return_position_x();
	double Return_position_y();
	double Return_speed_x();
	double Return_speed_y();
	double Return_radius();
	double Return_force_x();
	double Return_force_y();
	double Return_density();
	void Add_volume(int);
	void Add_charge(int);
	void Change_radiusbyvolume(int);
	void Decide_force_x(double);
	void Decide_force_y(double);
	void Decide_speed_x(double);
	void Decide_speed_y(double);
};

//動かない引力なしのボール
class NonMovableBall {
	double radius;
	unsigned int volume_text_color;
protected:
	int volume;
	unsigned int own_color;
	double position_x, position_y;
	double density;
public:
	NonMovableBall(double, double);				//引数(初期x座標,初期y座標)
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
	int charge;
	double speed_x, speed_y;
	double acceleration_x, acceleration_y;
	double force_x, force_y;
public:
	MovableChargedBall(double, double);
	~MovableChargedBall();
	void Update();
	int Return_charge();
	double Return_speed_x();
	double Return_speed_y();
	void Decide_force_x(double);
	void Decide_force_y(double);
};
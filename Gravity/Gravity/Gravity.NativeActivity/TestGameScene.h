#pragma once
#include "DxLib.h"
#include "define.h"
#include "SceneBase.h"
#include "Actor.h"
#include <memory>
#include <math.h>

class TestGameScene :public SceneBase {
	double air_resistance_coefficient;
	std::shared_ptr<Player> player;
	std::shared_ptr<NonMovableBall> size_up_ball[3];
	std::shared_ptr<MovableChargedBall> charged_ball[3];
public:
	TestGameScene();
	~TestGameScene();
	void Update();
	void Draw()const;
	void HitConbine();
	bool HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player>, std::shared_ptr<NonMovableBall>);
	bool HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player>, std::shared_ptr<MovableChargedBall>);
	bool HitChecker_MovableChargedBallandNonMovableBall(std::shared_ptr<MovableChargedBall>, std::shared_ptr<NonMovableBall>);
	void Gravity();
	void AirResistance();
};
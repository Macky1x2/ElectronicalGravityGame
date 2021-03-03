#pragma once
#include "DxLib.h"
#include "define.h"
#include "SceneBase.h"
#include "Actor.h"
#include "Operation.h"
#include <math.h>

class TestGameScene :public SceneBase {
	bool time_advances;
	double air_resistance_coefficient;
	std::shared_ptr<Player> player;
	std::shared_ptr<NonMovableBall> size_up_ball[3];
	std::shared_ptr<MovableChargedBall> charged_ball[3];
	std::shared_ptr<OperationInGame> operate;
public:
	TestGameScene();
	~TestGameScene();
	void Update();
	void Draw()const;
	void HitConbine();
	bool HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player>, std::shared_ptr<NonMovableBall>);
	bool HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player>, std::shared_ptr<MovableChargedBall>);
	bool HitChecker_MovableChargedBallandNonMovableBall(std::shared_ptr<MovableChargedBall>, std::shared_ptr<NonMovableBall>);
	bool ClearChecker();
	void Gravity();
	void AirResistance();
	void TimeControl();
	void GameClear();
};
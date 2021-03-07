#pragma once
#include "StageSelectScene.h"
#include "Actor.h"
#include "Operation.h"

class GameBaseScene :public SceneBase {
	std::shared_ptr<OperationInGame> operate;
protected:
	bool time_advances;
	double air_resistance_coefficient;
	int player_num, size_up_ball_num, charged_ball_num;
	int accel_arrowGHandle;
	int charge_THandle;
	std::shared_ptr<Player>* player;
	std::shared_ptr<NonMovableBall>* size_up_ball;
	std::shared_ptr<MovableChargedBall>* charged_ball;
public:
	GameBaseScene();
	~GameBaseScene();
	void Update();
	void Draw()const;
	void HitConbine();
	bool HitChecker_PlayerandPlayer(std::shared_ptr<Player>, std::shared_ptr<Player>);
	bool HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player>, std::shared_ptr<NonMovableBall>);
	bool HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player>, std::shared_ptr<MovableChargedBall>);
	bool HitChecker_MovableChargedBallandNonMovableBall(std::shared_ptr<MovableChargedBall>, std::shared_ptr<NonMovableBall>);
	virtual bool ClearChecker();
	void Gravity();
	void AirResistance();
	void TimeControl();
	virtual void GameClear();
	void ReloadFunction(void);
};
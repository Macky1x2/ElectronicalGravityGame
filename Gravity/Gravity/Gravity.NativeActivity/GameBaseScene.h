#pragma once
#include "StageSelectScene.h"
#include "Actor.h"

class GameBaseScene :public SceneBase {
	std::shared_ptr<OperationInGame> operate;
protected:
	bool time_advances;
	double air_resistance_coefficient;
	int player_num, size_up_ball_num, charged_ball_num;
	int accel_arrowGHandle, playerGHandle, charged_ballGHandle;
	int phase, star;
	int fade_in, fade_in_speed, fade_out, fade_out_speed;
	string purpose;
	std::shared_ptr<Player>* player;
	std::shared_ptr<NonMovableBall>* size_up_ball;
	std::shared_ptr<MovableChargedBall>* charged_ball;
public:
	GameBaseScene();
	~GameBaseScene();
	void Update();
	void Draw()const;
	void Draw_Objects()const;
	void HitConbine();
	bool HitChecker_PlayerandPlayer(std::shared_ptr<Player>, std::shared_ptr<Player>);
	bool HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player>, std::shared_ptr<NonMovableBall>);
	bool HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player>, std::shared_ptr<MovableChargedBall>);
	bool HitChecker_MovableChargedBallandNonMovableBall(std::shared_ptr<MovableChargedBall>, std::shared_ptr<NonMovableBall>);
	virtual bool ClearChecker() = 0;
	void Gravity();
	void AirResistance();
	void TimeControl();
	virtual void GameClear() = 0;
	void ReloadFunction(void);
	void Draw_Purpose()const;
};
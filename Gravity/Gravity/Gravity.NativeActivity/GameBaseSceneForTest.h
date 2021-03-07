//テスト開発用
#pragma once
#include "StageSelectScene.h"
#include "Actor.h"
#include "Operation.h"

class GameBaseSceneForTest :public SceneBase {
	bool time_advances;
	std::shared_ptr<OperationInGame> operate;
protected:
	double air_resistance_coefficient;
	int player_num, size_up_ball_num, charged_ball_num;
	std::shared_ptr<Player>* player;
	std::shared_ptr<NonMovableBall>* size_up_ball;
	std::shared_ptr<MovableChargedBall>* charged_ball;
public:
	GameBaseSceneForTest();
	~GameBaseSceneForTest();
	void Update();
	void Draw()const;
	void HitConbine();
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
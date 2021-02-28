#pragma once
#include "DxLib.h"
#include "define.h"
#include "SceneBase.h"
#include "Actor.h"
#include <memory>
#include <math.h>

class TestGameScene :public SceneBase {
	std::shared_ptr<Player> player;
	std::shared_ptr<NonMovableBall> testball[3];
	std::shared_ptr<MovableChargedBall> testchargedball;
public:
	TestGameScene();
	~TestGameScene();
	void Update();
	void Draw()const;
	void HitConbine();
	bool HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player>, std::shared_ptr<NonMovableBall>);
	bool HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player>, std::shared_ptr<MovableChargedBall>);
	void Gravity();
};
#pragma once
#include "SceneManager.h"

class GameScene :public SceneBase {
	
public:
	GameScene();
	~GameScene();
	void Update();
	void Draw()const;
};
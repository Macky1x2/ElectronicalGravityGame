#pragma once
#include "SceneBase.h"

class StageSelectScene :public SceneBase {
public:
	StageSelectScene();
	~StageSelectScene();
	void Update();
	void Draw()const;
};
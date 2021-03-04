#pragma once
#include "TitleScene.h"

class StageSelectScene :public SceneBase {
public:
	StageSelectScene();
	~StageSelectScene();
	void Update();
	void Draw()const;
	void ReloadFunction(void);
};
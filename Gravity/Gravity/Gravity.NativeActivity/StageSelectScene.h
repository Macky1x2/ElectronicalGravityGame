#pragma once
#include "TitleScene.h"

class StageSelectScene :public SceneBase {
	int situation;
	int explainTHandle;
	int explain_color;
public:
	StageSelectScene();
	~StageSelectScene();
	void Update();
	void Draw()const;
	void ReloadFunction(void);
};
#pragma once
#include "SceneManager.h"

class TitleScene :public SceneBase {
	int Tap_THandle;
	int Tap_Color;
public:
	TitleScene();
	~TitleScene();
	void Update();
	void Draw()const;
	void ReloadFunction(void);
};
#pragma once
#include "SceneManager.h"

class TitleScene :public SceneBase {

public:
	TitleScene();
	~TitleScene();
	void Update();
	void Draw()const;
	void ReloadFunction(void);
};
#pragma once
#include "DxLib.h"
#include "define.h"
#include "SceneBase.h"
#include <memory>

class TitleScene :public SceneBase {

public:
	TitleScene();
	~TitleScene();
	void Update();
	void Draw()const;
};
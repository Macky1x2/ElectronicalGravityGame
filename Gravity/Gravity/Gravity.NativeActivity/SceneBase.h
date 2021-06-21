#pragma once
#include "DxLib.h"
#include "define.h"
#include <iostream>
#include <memory>
#include <math.h>
#include <vector>
#include <utility>

using namespace std;

class SceneBase {
	friend class SceneManager;
protected:
	//次のシーン(シーン遷移時に代入)
	shared_ptr<SceneBase> nextScene;
public:
	SceneBase(){}
	~SceneBase(){}
	virtual void Draw()const = 0;
	virtual void Update() = 0;
};
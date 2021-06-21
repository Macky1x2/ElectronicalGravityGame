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
	//���̃V�[��(�V�[���J�ڎ��ɑ��)
	shared_ptr<SceneBase> nextScene;
public:
	SceneBase(){}
	~SceneBase(){}
	virtual void Draw()const = 0;
	virtual void Update() = 0;
};
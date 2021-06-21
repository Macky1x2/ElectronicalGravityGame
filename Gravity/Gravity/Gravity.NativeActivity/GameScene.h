#pragma once
#include "SceneManager.h"

class GameScene :public SceneBase {
	int backGHandle;
	unsigned int Circle_Color[5];
	int p_touch_x, p_touch_y;
	int board[8][8] = {};
	int order[5];
	int order_count;
	int vec_x[8] = { 0,1,1,1,0,-1,-1,-1 };
	int vec_y[8] = { -1,-1,0,1,1,1,0,-1 };
public:
	GameScene();
	~GameScene();
	void Update();
	void Draw()const;
	void Draw_Background()const;
	void Draw_Piece()const;
	void Player();
	void Enemy();
	bool Can_put(int, int, int);
	bool Can_put_details(int, int, int, int);		//最後の引数: vec_x[i]のi
	void Next_order();
	void Board_change(int, int, int);
};
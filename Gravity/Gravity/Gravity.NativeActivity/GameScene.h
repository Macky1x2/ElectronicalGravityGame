#pragma once
#include "SceneManager.h"

class Board {
public:
	int board[8][8] = {};
};

class GameScene :public SceneBase {
	int backGHandle;
	unsigned int Circle_Color[5];
	int p_touch_x, p_touch_y;
	int board[8][8] = {};
	int order[5];
	int order_count;
	int vec_x[8] = { 0,1,1,1,0,-1,-1,-1 };
	int vec_y[8] = { -1,-1,0,1,1,1,0,-1 };
	Board give_board;
public:
	GameScene();
	~GameScene();
	void Update();
	void Draw()const;
	void Draw_Background()const;
	void Draw_Piece()const;
	void Player();
	void Enemy();
	int Enemy_cal(Board, int, int, int, int, bool, int);
	bool Can_put(int, int, int, int _board[8][8]);
	bool Can_put_details(int, int, int, int, int _board[8][8]);		//最後の引数: vec_x[i]のi
	void Next_order(int*);
	int Return_Next_order(int);
	void Board_change(int, int, int, int _board[8][8]);
	int Evaluate_board(int _board[8][8], int);
};
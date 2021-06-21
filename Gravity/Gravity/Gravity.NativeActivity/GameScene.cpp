#include "GameScene.h"

GameScene::GameScene() {
	board[2][3] = 1;
	board[3][4] = 1;
	board[3][5] = 2;
	board[4][4] = 2;
	board[5][4] = 3;
	board[4][3] = 3;
	board[4][2] = 4;
	board[3][3] = 4;
	backGHandle = MakeScreen(1080, 1920, FALSE);
	Circle_Color[1] = GetColor(255, 255, 255);
	Circle_Color[2] = GetColor(255, 0, 0);
	Circle_Color[3] = GetColor(0, 0, 0);
	Circle_Color[4] = GetColor(0, 0, 255);
	order_count = 1;
	order[0] = 0;
	for (int i = 1; i <= 4; i++) {
		while (1) {
			int temp = GetRand(4);
			bool can_use = true;
			for (int j = 0; j < i; j++) {
				if (temp == order[j]) {
					can_use = false;
					break;
				}
			}
			if (can_use) {
				order[i] = temp;
				break;
			}
		}
	}
	SetDrawScreen(backGHandle);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			DrawBox(2 + 135 * j, 422 + 135 * i, 2 + 135 * j + 131, 422 + 135 * i + 131, GetColor(0, 255, 0), TRUE);
		}
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

GameScene::~GameScene() {
	
}

void GameScene::Update() {
	if (order[order_count] == 1) {
		Player();
	}
	else {
		int start_time = GetNowCount();
		Enemy();
		int end_time = GetNowCount();
		if (end_time - start_time < 1000) {
			WaitTimer(1000 - (end_time - start_time));
		}
		Next_order();
	}
}

void GameScene::Player() {
	if (GetTouchInputNum() == 1) {
		GetTouchInput(0, &p_touch_x, &p_touch_y, NULL, NULL);
		int loc_x = p_touch_x / 135;
		int loc_y = (p_touch_y - 420) / 135;
		if (loc_x >= 0 && loc_x < 8 && loc_y >= 0 && loc_y < 8) {
			if (Can_put(loc_x, loc_y, 1)) {
				Board_change(loc_x, loc_y, 1);
				Next_order();
			}
		}
	}
}

void GameScene::Enemy() {
	vector<pair<int, int> > can_place;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (Can_put(j, i, order[order_count])) {
				can_place.push_back(make_pair(j, i));
			}
		}
	}
	if (can_place.size() > 0) {
		pair<int, int> res = can_place[GetRand(can_place.size() - 1)];
		Board_change(res.first, res.second, order[order_count]);
	}
}

void GameScene::Draw()const {
	Draw_Background();
	Draw_Piece();
}

void GameScene::Draw_Background()const {
	DrawGraph(0, 0, backGHandle, FALSE);
}

void GameScene::Draw_Piece()const {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] != 0) {
				DrawCircle(67 + 135 * j, 487 + 135 * i, 60, Circle_Color[board[i][j]], TRUE);
			}
		}
	}
}

bool GameScene::Can_put(int _x, int _y, int who) {
	if (board[_y][_x] != 0)return false;
	for (int i = 0; i < 8; i++) {
		if (Can_put_details(_x, _y, who, i)) {
			return true;
		}
	}
	return false;
}

bool GameScene::Can_put_details(int _x, int _y, int who, int vec_type) {
	int x = _x + vec_x[vec_type];
	int y = _y + vec_y[vec_type];
	int temp_x = x, temp_y = y;
	bool first_checker = false;
	while (temp_x >= 0 && temp_x < 8 && temp_y >= 0 && temp_y < 8) {
		if (board[temp_y][temp_x] == 0)break;
		if (!first_checker) {
			if (board[temp_y][temp_x] != who) {
				first_checker = true;
			}
			else {
				break;
			}
		}
		else {
			if (board[temp_y][temp_x] == who) {
				return true;
			}
		}
		temp_x += vec_x[vec_type];
		temp_y += vec_y[vec_type];
	}
	return false;
}

void GameScene::Next_order() {
	if (order_count == 4)order_count = 1;
	else order_count++;
}

void GameScene::Board_change(int _x, int _y, int who) {
	board[_y][_x] = who;
	for (int i = 0; i < 8; i++) {
		if (Can_put_details(_x, _y, who, i)) {
			int temp_x = _x + vec_x[i], temp_y = _y + vec_y[i];
			while (board[temp_y][temp_x] != who) {
				board[temp_y][temp_x] = who;
				temp_x += vec_x[i];
				temp_y += vec_y[i];
			}
		}
	}
}
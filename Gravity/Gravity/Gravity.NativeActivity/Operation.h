#pragma once
#include "DxLib.h"
#include "define.h"
#include <memory>

class OperationInGame {
	int touch_num_now, touch_num_pre;
	int touch_positionX[4], touch_positionY[4];
	int one_touch_frame, one_touch_frame_result, one_touch_start_x, one_touch_start_y, one_touch_result_distance2;
	bool checker_first;
public:
	OperationInGame();
	~OperationInGame();
	void Update();
	void Draw()const;
	int Return_one_touch_frame_result();
	int Return_one_touch_result_distance2();
};

class SquareButton {
	int touch_num_pre;
	int pre_touch_x, pre_touch_y;
	bool Checker_specific_func_result;
	int leftupX, leftupY, W, H;
public:
	SquareButton(int, int, int, int);
	~SquareButton();
	bool Checker_specific_place_touch_in_out();
};

class ReverseSquareButton {
	std::shared_ptr<SquareButton> button[4];
public:
	ReverseSquareButton(int, int, int, int);
	~ReverseSquareButton();
	bool Checker_reverse_specific_place_touch_in_out();
};
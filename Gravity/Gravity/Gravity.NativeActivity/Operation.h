#pragma once
#include "DxLib.h"

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
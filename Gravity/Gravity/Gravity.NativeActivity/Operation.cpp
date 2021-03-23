#include "Operation.h"

OperationInGame::OperationInGame() {
	touch_num_pre = 0;
	checker_first = false;
	one_touch_frame = -1;
	one_touch_frame_result = -1;
}

OperationInGame::~OperationInGame() {

}

void OperationInGame::Update() {
	touch_num_now = GetTouchInputNum();												//現在のタッチ数を取得
	if (one_touch_frame_result != -1) {
		one_touch_frame_result = -1;												//one_touchの結果出力後リセット
	}

	//手を1度離してからゲームを開始させるためのchecker
	if (!checker_first && touch_num_pre == 0) {
		checker_first = true;
	}

	//タッチ各種情報を取得
	for (int i = 0; i < touch_num_now; i++) {
		GetTouchInput(i, &touch_positionX[i], &touch_positionY[i], NULL, NULL);
	}

	//1箇所タッチし始める時～1箇所タッチし終えるときの時間差を計測(タップによる時間停止・進行に使う),2点間(開始点,終了点)の距離も計測
	if (one_touch_frame != -1) {
		if (touch_num_pre == 1) {
			if (touch_num_now == 1) {
				one_touch_frame++;
			}
			else if (touch_num_now > 1) {
				one_touch_frame = -1;
			}
		}
	}
	if (touch_num_pre == 0 && touch_num_now == 1) {
		one_touch_frame = 0;
		one_touch_start_x = touch_positionX[0];
		one_touch_start_y = touch_positionY[0];
	}
	if (touch_num_pre == 1 && touch_num_now == 0&& checker_first) {
		one_touch_frame_result = one_touch_frame;
		one_touch_result_distance2 = (touch_positionX[0] - one_touch_start_x) * (touch_positionX[0] - one_touch_start_x) + (touch_positionY[0] - one_touch_start_y) * (touch_positionY[0] - one_touch_start_y);
		one_touch_frame = -1;
	}

	touch_num_pre = touch_num_now;
}

void OperationInGame::Draw()const {
	
}

int OperationInGame::Return_one_touch_frame_result() {
	return one_touch_frame_result;
}

int OperationInGame::Return_one_touch_result_distance2() {
	return one_touch_result_distance2;
}

SquareButton::SquareButton(int _leftupX, int _leftupY, int _W, int _H) {
	leftupX = _leftupX;
	leftupY = _leftupY;
	W = _W;
	H = _H;
	touch_num_pre = 0;
	Checker_specific_func_result = false;
}

SquareButton::~SquareButton() {

}

//指定された範囲内をタップし、指定された範囲内で指を離せばtrueを返すボタン
bool SquareButton::Checker_specific_place_touch_in_out() {
	if (GetTouchInputNum() == 1) {
		int x, y;
		GetTouchInput(0, &x, &y, NULL, NULL);
		pre_touch_x = x;
		pre_touch_y = y;
		if (touch_num_pre == 0 && leftupX <= x && x < leftupX + W && leftupY <= y && y < leftupY + H) {
			Checker_specific_func_result = true;
		}
		touch_num_pre = 1;
	}
	else {
		if (Checker_specific_func_result) {
			if (GetTouchInputNum() == 0 && leftupX <= pre_touch_x && pre_touch_x < leftupX + W && leftupY <= pre_touch_y && pre_touch_y < leftupY + H) {
				Checker_specific_func_result = false;
				touch_num_pre = 0;
				return true;
			}
			else {
				Checker_specific_func_result = false;
			}
		}
		touch_num_pre = GetTouchInputNum();
	}
	return false;
}

ReverseSquareButton::ReverseSquareButton(int leftupX, int leftupY, int W, int H) {
	button[0] = std::make_shared<SquareButton>(0, 0, leftupX, ANDROID_HEIGHT);
	button[1] = std::make_shared<SquareButton>(0, 0, ANDROID_WIDTH, leftupY);
	button[2] = std::make_shared<SquareButton>(leftupX + W, 0, ANDROID_WIDTH - (leftupX + W), ANDROID_HEIGHT);
	button[3] = std::make_shared<SquareButton>(0, leftupY + H, ANDROID_WIDTH, ANDROID_HEIGHT - (leftupY + H));
}

ReverseSquareButton::~ReverseSquareButton() {
	for (int i = 0; i < 4; i++) {
		if (button[i]) {
			button[i].reset();
		}
	}
}

//SquareButtonの、指定された範囲外バージョン
bool ReverseSquareButton::Checker_reverse_specific_place_touch_in_out() {
	if (button[0]->Checker_specific_place_touch_in_out() || button[1]->Checker_specific_place_touch_in_out() || button[2]->Checker_specific_place_touch_in_out() || button[3]->Checker_specific_place_touch_in_out()) {
		return true;
	}
	else {
		return false;
	}
}
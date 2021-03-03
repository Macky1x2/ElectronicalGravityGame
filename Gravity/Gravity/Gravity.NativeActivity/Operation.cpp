#include "Operation.h"

OperationInGame::OperationInGame() {
	touch_num_pre = 1;
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

	//1箇所タッチし始める時～1箇所タッチし終えるときの時間差を計測(タップによる時間停止・進行に使う)
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
	}
	if (touch_num_pre != 0 && touch_num_now == 0) {
		one_touch_frame_result = one_touch_frame;
		one_touch_frame = -1;
	}
	printfDx("%d", one_touch_frame);
	if (one_touch_frame_result != -1) {
		printfDx(" %d", one_touch_frame_result);
	}
	printfDx("\n");

	touch_num_pre = touch_num_now;
}

void OperationInGame::Draw()const {
	
}

int OperationInGame::Return_one_touch_frame_result() {
	return one_touch_frame_result;
}
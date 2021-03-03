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
	touch_num_now = GetTouchInputNum();												//���݂̃^�b�`�����擾
	if (one_touch_frame_result != -1) {
		one_touch_frame_result = -1;												//one_touch�̌��ʏo�͌ナ�Z�b�g
	}

	//���1�x�����Ă���Q�[�����J�n�����邽�߂�checker
	if (!checker_first && touch_num_pre == 0) {
		checker_first = true;
	}

	//�^�b�`�e������擾
	for (int i = 0; i < touch_num_now; i++) {
		GetTouchInput(i, &touch_positionX[i], &touch_positionY[i], NULL, NULL);
	}

	//1�ӏ��^�b�`���n�߂鎞�`1�ӏ��^�b�`���I����Ƃ��̎��ԍ����v��(�^�b�v�ɂ�鎞�Ԓ�~�E�i�s�Ɏg��),2�_��(�J�n�_,�I���_)�̋������v��
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
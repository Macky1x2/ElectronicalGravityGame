#include <iostream>
#include "DxLib.h"
using namespace std;

//�}�N���l
#define ROOT_2 1.41421356

//�O���[�o���ϐ�
int scene;

//�摜�E����N���X
class Graph {
public:
	int aka_sei_3;
	int ki_sei_3;
	int aka_hisei_3_1;
	Graph() {
		aka_sei_3 = LoadGraph("aka_sei3.png");
		ki_sei_3 = LoadGraph("ki_sei3.png");
		aka_hisei_3_1 = LoadGraph("aka_hisei3_1.png");
	}
};

//���N���X
class Sound {
public:
};

//�����N���X
class Text {
public:
};

//����L�����N���X
class Player {
public:
	double x;
	double y;
	int mitame;
	double movespeed;
	void Start() {
		x = 540.0;
		y = 1600.0;
	}
};

//�ۑ����N���X
class Information {
public:
};

//�֐��錾
int title();//�^�C�g��
int game(Graph*, Sound*, Text*);//�Q�[��
void player(Graph*, Sound*, Text*, Player*);//����L��������
void player_move1(Graph*, Sound*, Text*, Player*);//����L�����̈ړ�1
void player_move2(Graph*, Sound*, Text*, Player*);//����L�����̈ړ�2


//���S����
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	//����������
	if (DxLib_Init()==-1) {
		return -1;
	}
	//�Q�[������
	SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(1920, 1080, 32);
	Graph G;
	Sound S;
	Text T;
	scene = 2;
	switch (scene) {
	case 1:title();
	case 2:scene = game(&G, &S, &T);
	case 0://�I������
		DxLib_End();
		return 0;
	}
}

//�Q�[���`�揈��
int game(Graph* g, Sound* s, Text* t) {
	Player p;
	p.Start();
	p.movespeed = 3.0;
	p.mitame = g->ki_sei_3;
	while (ProcessMessage()!=-1) {
		ClearDrawScreen();
		player(g, s, t, &p);
		DrawRotaGraph(p.x, p.y, 1.0, 0.0, p.mitame, TRUE, FALSE);
		ScreenFlip();
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			return 0;
		}
	}
	return 0;
}

//����L������������
void player(Graph* g, Sound* s, Text* t, Player* p) {
	player_move1(g, s, t, p);
	player_move2(g, s, t, p);
}

//����L�����ړ�����1
void player_move1(Graph* g, Sound* s, Text* t,Player* p) {
	if (CheckHitKey(KEY_INPUT_D) == 1&& CheckHitKey(KEY_INPUT_W) == 0 && CheckHitKey(KEY_INPUT_S) == 0) {
		p->x += p->movespeed;
		return;
	}
	if (CheckHitKey(KEY_INPUT_W) == 1 && CheckHitKey(KEY_INPUT_A) == 0 && CheckHitKey(KEY_INPUT_D) == 0) {
		p->y -= p->movespeed;
		return;
	}
	if (CheckHitKey(KEY_INPUT_A) == 1 && CheckHitKey(KEY_INPUT_W) == 0 && CheckHitKey(KEY_INPUT_S) == 0) {
		p->x -= p->movespeed;
		return;
	}
	if (CheckHitKey(KEY_INPUT_S) == 1 && CheckHitKey(KEY_INPUT_A) == 0 && CheckHitKey(KEY_INPUT_D) == 0) {
		p->y += p->movespeed;
		return;
	}
	if (CheckHitKey(KEY_INPUT_D) == 1 && CheckHitKey(KEY_INPUT_W) == 1) {
		p->x += (p->movespeed / ROOT_2);
		p->y -= (p->movespeed / ROOT_2);
		return;
	}
	if (CheckHitKey(KEY_INPUT_W) == 1 && CheckHitKey(KEY_INPUT_A) == 1) {
		p->x -= (p->movespeed / ROOT_2);
		p->y -= (p->movespeed / ROOT_2);
		return;
	}
	if (CheckHitKey(KEY_INPUT_A) == 1 && CheckHitKey(KEY_INPUT_S) == 1) {
		p->x -= (p->movespeed / ROOT_2);
		p->y += (p->movespeed / ROOT_2);
		return;
	}
	if (CheckHitKey(KEY_INPUT_D) == 1 && CheckHitKey(KEY_INPUT_S) == 1) {
		p->x += (p->movespeed / ROOT_2);
		p->y += (p->movespeed / ROOT_2);
		return;
	}
}

//����L�����ړ�����2
void player_move2(Graph* g, Sound* s, Text* t, Player* p) {
	if (p->x > 1919) {
		p->x = 1919.0;
	}
	if (p->x < 0) {
		p->x = 0.0;
	}
	if (p->y > 1079) {
		p->y = 1079.0;
	}
	if (p->y < 0) {
		p->y = 0.0;
	}
}

//�^�C�g������
int title() {
	return 0;
}
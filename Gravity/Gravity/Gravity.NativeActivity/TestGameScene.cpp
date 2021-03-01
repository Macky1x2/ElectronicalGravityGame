#include "TestGameScene.h"

TestGameScene::TestGameScene() {
	player = std::make_shared<Player>();
	size_up_ball[0] = std::make_shared<NonMovableBall>(300, 1000);		//初期座標は別ファイルから読み取るのがいいかもしれない
	size_up_ball[1] = std::make_shared<NonMovableBall>(600, 1500);
	size_up_ball[2] = std::make_shared<NonMovableBall>(1050, 300);
	charged_ball[0] = std::make_shared<MovableChargedBall>(750, 400);
	charged_ball[1] = std::make_shared<MovableChargedBall>(100, 1100);
	charged_ball[2] = std::make_shared<MovableChargedBall>(250, 1700);
}

TestGameScene::~TestGameScene() {

}

void TestGameScene::HitConbine() {
	//プレイヤーと動かないボール間について
	for (int i = 0; i < 3; i++) {
		if (player && size_up_ball[i]) {
			if (HitChecker_PlayerandNonMovableBall(player, size_up_ball[i])) {
				player->Add_volume(size_up_ball[i]->Return_volume());
				size_up_ball[i].reset();
			}
		}
	}

	//プレイヤーと可動な電気を帯びたボール間について
	for (int i = 0; i < 3; i++) {
		if (player && charged_ball[i]) {
			if (HitChecker_PlayerandMovableChargedBall(player, charged_ball[i])) {
				//運動量保存則
				double m_p = player->Return_density() * player->Return_volume() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume();
				player->Decide_speed_x((player->Return_density() * player->Return_volume() * player->Return_speed_x() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_x()) / m_p);
				player->Decide_speed_y((player->Return_density() * player->Return_volume() * player->Return_speed_y() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_y()) / m_p);
				player->Decide_force_x(0.0);
				player->Decide_force_y(0.0);

				//プレイヤー側に加算
				player->Add_volume(charged_ball[i]->Return_volume());
				player->Add_charge(charged_ball[i]->Return_charge());
				charged_ball[i].reset();
			}
		}
	}
}

void TestGameScene::Gravity() {
	//初期化
	if (player) {
		player->Decide_force_x(0.0);
		player->Decide_force_y(0.0);
	}
	for (int i = 0; i < 3; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Decide_force_x(0.0);
			charged_ball[i]->Decide_force_y(0.0);
		}
	}

	//プレイヤー・電荷を帯びた可動なボール間の引力
	for (int i = 0; i < 3; i++) {
		if (player && charged_ball[i]) {
			double r = pow((charged_ball[i]->Return_position_x() - player->Return_position_x()) * (charged_ball[i]->Return_position_x() - player->Return_position_x()) + (charged_ball[i]->Return_position_y() - player->Return_position_y()) * (charged_ball[i]->Return_position_y() - player->Return_position_y()), 1.0 / 2);
			player->Decide_force_x(-COULOMB_CONSTANT * player->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_x() - player->Return_position_x()) / (r * r * r));
			player->Decide_force_y(-COULOMB_CONSTANT * player->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_y() - player->Return_position_y()) / (r * r * r));
			charged_ball[i]->Decide_force_x(-player->Return_force_x());
			charged_ball[i]->Decide_force_y(-player->Return_force_y());
		}
	}
}

void TestGameScene::Update() {
	Gravity();					//引力処理
	if (player) {
		player->Update();
	}
	for (int i = 0; i < 3; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Update();
		}
	}
	HitConbine();				//衝突then結合処理
}

void TestGameScene::Draw()const {
	for (int i = 0; i < 3; i++) {
		if (size_up_ball[i]) {
			size_up_ball[i]->Draw();
		}
	}
	for (int i = 0; i < 3; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Draw();
		}
	}
	if (player) {
		player->Draw();
	}
}

bool TestGameScene::HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player> _player, std::shared_ptr<NonMovableBall> _size_up_ball) {
	//if(当たっているならば)
	if ((_player->Return_position_x() - _size_up_ball->Return_position_x()) * (_player->Return_position_x() - _size_up_ball->Return_position_x()) + (_player->Return_position_y() - _size_up_ball->Return_position_y()) * (_player->Return_position_y() - _size_up_ball->Return_position_y()) < (_player->Return_radius() + _size_up_ball->Return_radius()) * (_player->Return_radius() + _size_up_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool TestGameScene::HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player> _player, std::shared_ptr<MovableChargedBall> _charged_ball) {
	//if(当たっているならば)
	if ((_player->Return_position_x() - _charged_ball->Return_position_x()) * (_player->Return_position_x() - _charged_ball->Return_position_x()) + (_player->Return_position_y() - _charged_ball->Return_position_y()) * (_player->Return_position_y() - _charged_ball->Return_position_y()) < (_player->Return_radius() + _charged_ball->Return_radius()) * (_player->Return_radius() + _charged_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}
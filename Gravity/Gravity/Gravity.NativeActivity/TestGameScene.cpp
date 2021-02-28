#include "TestGameScene.h"

TestGameScene::TestGameScene() {
	player = std::make_shared<Player>();
	testball[0] = std::make_shared<NonMovableBall>(300, 1000);		//初期座標は別ファイルから読み取るのがいいかもしれない
	testball[1] = std::make_shared<NonMovableBall>(600, 1500);
	testball[2] = std::make_shared<NonMovableBall>(1050, 300);
	testchargedball = std::make_shared<MovableChargedBall>(750, 1200);
}

TestGameScene::~TestGameScene() {

}

void TestGameScene::HitConbine() {
	//プレイヤーと動かないボール間について
	for (int i = 0; i < 3; i++) {
		if (player && testball[i]) {
			if (HitChecker_PlayerandNonMovableBall(player, testball[i])) {
				player->Add_volume(testball[i]->Return_volume());
				testball[i].reset();
			}
		}
	}

	//プレイヤーと可動な電気を帯びたボール間について
	if (player && testchargedball) {
		if (HitChecker_PlayerandMovableChargedBall(player, testchargedball)) {
			player->Add_volume(testchargedball->Return_volume());
			testchargedball.reset();
		}
	}
}

void TestGameScene::Gravity() {
	//初期化
	if (player) {
		player->Decide_force_x(0.0);
		player->Decide_force_y(0.0);
	}
	if (testchargedball) {
		testchargedball->Decide_force_x(0.0);
		testchargedball->Decide_force_y(0.0);
	}

	//プレイヤー・電荷を帯びた可動なボール間の引力
	if (player && testchargedball) {
		double r = pow((testchargedball->Return_position_x() - player->Return_position_x()) * (testchargedball->Return_position_x() - player->Return_position_x()) + (testchargedball->Return_position_y() - player->Return_position_y()) * (testchargedball->Return_position_y() - player->Return_position_y()), 1.0 / 2);
		player->Decide_force_x(-COULOMB_CONSTANT * player->Return_charge() * testchargedball->Return_charge() * (testchargedball->Return_position_x() - player->Return_position_x()) / (r * r * r));
		player->Decide_force_y(-COULOMB_CONSTANT * player->Return_charge() * testchargedball->Return_charge() * (testchargedball->Return_position_y() - player->Return_position_y()) / (r * r * r));
		testchargedball->Decide_force_x(-player->Return_force_x());
		testchargedball->Decide_force_y(-player->Return_force_y());
	}
}

void TestGameScene::Update() {
	Gravity();					//引力処理
	if (player) {
		player->Update();
	}
	if (testchargedball) {
		testchargedball->Update();
	}
	HitConbine();				//衝突then結合処理
}

void TestGameScene::Draw()const {
	for (int i = 0; i < 3; i++) {
		if (testball[i]) {
			testball[i]->Draw();
		}
	}
	if (testchargedball) {
		testchargedball->Draw();
	}
	if (player) {
		player->Draw();
	}
}

bool TestGameScene::HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player> _player, std::shared_ptr<NonMovableBall> _testball) {
	//if(当たっているならば)
	if ((_player->Return_position_x() - _testball->Return_position_x()) * (_player->Return_position_x() - _testball->Return_position_x()) + (_player->Return_position_y() - _testball->Return_position_y()) * (_player->Return_position_y() - _testball->Return_position_y()) < (_player->Return_radius() + _testball->Return_radius()) * (_player->Return_radius() + _testball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool TestGameScene::HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player> _player, std::shared_ptr<MovableChargedBall> _testchargedball) {
	//if(当たっているならば)
	if ((_player->Return_position_x() - _testchargedball->Return_position_x()) * (_player->Return_position_x() - _testchargedball->Return_position_x()) + (_player->Return_position_y() - _testchargedball->Return_position_y()) * (_player->Return_position_y() - _testchargedball->Return_position_y()) < (_player->Return_radius() + _testchargedball->Return_radius()) * (_player->Return_radius() + _testchargedball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}
#include "TestGameScene.h"
#include "GameClearScene.h"

TestGameScene::TestGameScene() {
	time_advances = false;
	air_resistance_coefficient = 0.01;
	player = std::make_shared<Player>(500, 500, 5, 8, 0.5);						//引数(初期x座標, 初期y座標, 電荷, 体積, 密度)//初期座標は別ファイルから読み取るのがいいかもしれない
	size_up_ball[0] = std::make_shared<NonMovableBall>(300, 1000, 3, 0.5);				//引数(初期x座標, 初期y座標, 体積, 密度)
	size_up_ball[1] = std::make_shared<NonMovableBall>(600, 1500, 3, 0.5);
	size_up_ball[2] = std::make_shared<NonMovableBall>(1050, 300, 30, 0.5);
	charged_ball[0] = std::make_shared<MovableChargedBall>(750, 400, -5, 8, 0.5);			//引数(初期x座標, 初期y座標, 電荷, 体積, 密度)
	charged_ball[1] = std::make_shared<MovableChargedBall>(100, 1100, -5, 8, 0.5);
	charged_ball[2] = std::make_shared<MovableChargedBall>(250, 1700, -5, 8, 0.5);
	operate = std::make_shared<OperationInGame>();
}

TestGameScene::~TestGameScene() {
	if (player) {
		player.reset();
	}
	for (int i = 0; i < std::extent<decltype(size_up_ball), 0>::value; i++) {
		if (size_up_ball[i]) {
			size_up_ball[i].reset();
		}
	}
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (charged_ball[i]) {
			charged_ball[i].reset();
		}
	}
	operate.reset();
}

void TestGameScene::HitConbine() {
	//プレイヤーと動かないボール間について
	for (int i = 0; i < std::extent<decltype(size_up_ball), 0>::value; i++) {
		if (player && size_up_ball[i]) {
			if (HitChecker_PlayerandNonMovableBall(player, size_up_ball[i])) {
				//運動量保存則
				double m = player->Return_density() * player->Return_volume() + size_up_ball[i]->Return_density() * size_up_ball[i]->Return_volume();
				player->Decide_speed_x(player->Return_density() * player->Return_volume() * player->Return_speed_x() / m);
				player->Decide_speed_y(player->Return_density() * player->Return_volume() * player->Return_speed_y() / m);
				
				//プレイヤー側に加算
				//密度計算
				player->Decide_density((player->Return_density() * player->Return_volume() + size_up_ball[i]->Return_density() * size_up_ball[i]->Return_volume()) / (player->Return_volume() + size_up_ball[i]->Return_volume()));
				player->Add_volume(size_up_ball[i]->Return_volume());
				size_up_ball[i].reset();
			}
		}
	}

	//プレイヤーと可動な電気を帯びたボール間について
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (player && charged_ball[i]) {
			if (HitChecker_PlayerandMovableChargedBall(player, charged_ball[i])) {
				//運動量保存則
				double m = player->Return_density() * player->Return_volume() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume();
				player->Decide_speed_x((player->Return_density() * player->Return_volume() * player->Return_speed_x() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_x()) / m);
				player->Decide_speed_y((player->Return_density() * player->Return_volume() * player->Return_speed_y() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_y()) / m);

				//プレイヤー側に加算
				//密度計算
				player->Decide_density((player->Return_density() * player->Return_volume() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume()) / (player->Return_volume() + charged_ball[i]->Return_volume()));
				player->Add_volume(charged_ball[i]->Return_volume());
				player->Add_charge(charged_ball[i]->Return_charge());
				player->Make_TGHandle();
				charged_ball[i].reset();
			}
		}
	}

	//可動な電気を帯びたボールと動かないボール間について
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		for (int j = 0; j < std::extent<decltype(size_up_ball), 0>::value; j++) {
			if (charged_ball[i] && size_up_ball[j]) {
				if (HitChecker_MovableChargedBallandNonMovableBall(charged_ball[i], size_up_ball[j])) {
					//運動量保存則
					double m = charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() + size_up_ball[j]->Return_density() * size_up_ball[j]->Return_volume();
					charged_ball[i]->Decide_speed_x(charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_x() / m);
					charged_ball[i]->Decide_speed_y(charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_y() / m);

					//可動な電気を帯びたボール側に加算
					//密度計算
					charged_ball[i]->Decide_density((charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() + size_up_ball[j]->Return_density() * size_up_ball[j]->Return_volume()) / (charged_ball[i]->Return_volume() + size_up_ball[j]->Return_volume()));
					charged_ball[i]->Add_volume(size_up_ball[j]->Return_volume());
					size_up_ball[j].reset();
				}
			}
		}
	}

	//可動な電気を帯びたボール同士について
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		for (int j = i + 1; j < std::extent<decltype(charged_ball), 0>::value; j++) {
			if (charged_ball[i] && charged_ball[j]) {
				if (HitChecker_MovableChargedBallandNonMovableBall(charged_ball[i], charged_ball[j])) {		//MovableChargedBallはNonMovableBallの継承クラス
					if (charged_ball[i]->Return_volume() >= charged_ball[j]->Return_volume()) {				//でかい方の位置を合体後のボールの位置とする
						//運動量保存則
						double m = charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume();
						charged_ball[i]->Decide_speed_x((charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_x() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume() * charged_ball[j]->Return_speed_x()) / m);
						charged_ball[i]->Decide_speed_y((charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_y() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume() * charged_ball[j]->Return_speed_y()) / m);

						//プレイヤー側に加算
						//密度計算
						charged_ball[i]->Decide_density((charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume()) / (charged_ball[i]->Return_volume() + charged_ball[j]->Return_volume()));
						charged_ball[i]->Add_volume(charged_ball[j]->Return_volume());
						charged_ball[i]->Add_charge(charged_ball[j]->Return_charge());
						charged_ball[i]->Make_TGHandle();
						charged_ball[j].reset();
					}
					else {
						//運動量保存則
						double m = charged_ball[j]->Return_density() * charged_ball[j]->Return_volume() + charged_ball[i]->Return_density() * charged_ball[i]->Return_volume();
						charged_ball[j]->Decide_speed_x((charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_x() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume() * charged_ball[j]->Return_speed_x()) / m);
						charged_ball[j]->Decide_speed_y((charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_y() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume() * charged_ball[j]->Return_speed_y()) / m);

						//プレイヤー側に加算
						//密度計算
						charged_ball[j]->Decide_density((charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume()) / (charged_ball[i]->Return_volume() + charged_ball[j]->Return_volume()));
						charged_ball[j]->Add_volume(charged_ball[i]->Return_volume());
						charged_ball[j]->Add_charge(charged_ball[i]->Return_charge());
						charged_ball[j]->Make_TGHandle();
						charged_ball[i].reset();
					}
				}
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
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Decide_force_x(0.0);
			charged_ball[i]->Decide_force_y(0.0);
		}
	}

	//プレイヤー・電荷を帯びた可動なボール間のクーロン力
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (player && charged_ball[i]) {
			double r = pow((charged_ball[i]->Return_position_x() - player->Return_position_x()) * (charged_ball[i]->Return_position_x() - player->Return_position_x()) + (charged_ball[i]->Return_position_y() - player->Return_position_y()) * (charged_ball[i]->Return_position_y() - player->Return_position_y()), 1.0 / 2);
			double temp_x, temp_y;
			temp_x = -COULOMB_CONSTANT * player->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_x() - player->Return_position_x()) / (r * r * r);
			temp_y = -COULOMB_CONSTANT * player->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_y() - player->Return_position_y()) / (r * r * r);
			player->Add_force_x(temp_x);
			player->Add_force_y(temp_y);
			charged_ball[i]->Add_force_x(-temp_x);
			charged_ball[i]->Add_force_y(-temp_y);
		}
	}

	//電荷を帯びた可動なボール同士のクーロン力
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		for (int j = i + 1; j < std::extent<decltype(charged_ball), 0>::value; j++) {
			if (charged_ball[i] && charged_ball[j]) {
				double r = pow((charged_ball[i]->Return_position_x() - charged_ball[j]->Return_position_x()) * (charged_ball[i]->Return_position_x() - charged_ball[j]->Return_position_x()) + (charged_ball[i]->Return_position_y() - charged_ball[j]->Return_position_y()) * (charged_ball[i]->Return_position_y() - charged_ball[j]->Return_position_y()), 1.0 / 2);
				double temp_x, temp_y;
				temp_x = -COULOMB_CONSTANT * charged_ball[j]->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_x() - charged_ball[j]->Return_position_x()) / (r * r * r);
				temp_y = -COULOMB_CONSTANT * charged_ball[j]->Return_charge() * charged_ball[i]->Return_charge() * (charged_ball[i]->Return_position_y() - charged_ball[j]->Return_position_y()) / (r * r * r);
				charged_ball[j]->Add_force_x(temp_x);
				charged_ball[j]->Add_force_y(temp_y);
				charged_ball[i]->Add_force_x(-temp_x);
				charged_ball[i]->Add_force_y(-temp_y);
			}
		}
	}
}

void TestGameScene::Update() {
	operate->Update();			//操作処理
	TimeControl();				//時間処理
	Gravity();					//クーロン力処理
	AirResistance();			//媒質抵抗処理
	if (time_advances) {		//時が止まっているときは以下は更新しない
		if (player) {
			player->Update();
		}
		for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
			if (charged_ball[i]) {
				charged_ball[i]->Update();
			}
		}
		HitConbine();				//衝突then結合処理
	}
	if (ClearChecker()) {			//クリア判定となればゲームクリア
		GameClear();
	}
}

void TestGameScene::Draw()const {
	for (int i = 0; i < std::extent<decltype(size_up_ball), 0>::value; i++) {
		if (size_up_ball[i]) {
			size_up_ball[i]->Draw();
		}
	}
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
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

bool TestGameScene::HitChecker_MovableChargedBallandNonMovableBall(std::shared_ptr<MovableChargedBall> _charged_ball, std::shared_ptr<NonMovableBall> _size_up_ball) {
	//if(当たっているならば)
	if ((_charged_ball->Return_position_x() - _size_up_ball->Return_position_x()) * (_charged_ball->Return_position_x() - _size_up_ball->Return_position_x()) + (_charged_ball->Return_position_y() - _size_up_ball->Return_position_y()) * (_charged_ball->Return_position_y() - _size_up_ball->Return_position_y()) < (_charged_ball->Return_radius() + _size_up_ball->Return_radius()) * (_charged_ball->Return_radius() + _size_up_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool TestGameScene::ClearChecker() {
	if (player->Return_charge() <= -5) {
		return true;
	}
	else {
		return false;
	}
}

void TestGameScene::AirResistance() {
	//プレイヤーについて
	if (player) {
		player->Add_force_x(-air_resistance_coefficient * player->Return_speed_x());
		player->Add_force_y(-air_resistance_coefficient * player->Return_speed_y());
	}

	//電荷を帯びた可動なボールについて
	for (int i = 0; i < std::extent<decltype(charged_ball), 0>::value; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Add_force_x(-air_resistance_coefficient * charged_ball[i]->Return_speed_x());
			charged_ball[i]->Add_force_y(-air_resistance_coefficient * charged_ball[i]->Return_speed_y());
		}
	}
}

void TestGameScene::TimeControl() {
	//画面をタップすると時間停止・進行が切り替わる
	if (operate->Return_one_touch_frame_result() != -1) {
		if (operate->Return_one_touch_frame_result() <= 10) {
			if (operate->Return_one_touch_result_distance2() < 10) {
				if (time_advances) {
					time_advances = false;
				}
				else {
					time_advances = true;
				}
			}
		}
	}
	
	//時間停止時、プレイヤーショットをすると時が動き出す
	if (!time_advances) {
		player->Shoot_Operation();
		if (player->Return_checker_when_time_stopped()) {
			time_advances = true;
		}
	}
}

void TestGameScene::GameClear() {
	int star;								//3:星3つ, 2:星2つ, 1:星1つ
	if (player->Return_shoot_num() <= 2) {
		star = 3;
	}
	else if (player->Return_shoot_num() <= 4) {
		star = 2;
	}
	else {
		star = 1;
	}
	nextScene = make_shared<GameClearScene>(star);
}
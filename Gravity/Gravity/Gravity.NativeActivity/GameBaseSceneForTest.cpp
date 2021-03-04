//テスト開発用
#include "GameBaseSceneForTest.h"
#include "GameClearScene.h"

extern SceneBase* Scene_pointer_for_Reload;

GameBaseSceneForTest::GameBaseSceneForTest() {
	time_advances = false;
	operate = std::make_shared<OperationInGame>();
	Scene_pointer_for_Reload = this;
}

GameBaseSceneForTest::~GameBaseSceneForTest() {
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			player[i].reset();
		}
	}
	for (int i = 0; i < size_up_ball_num; i++) {
		if (size_up_ball[i]) {
			size_up_ball[i].reset();
		}
	}
	for (int i = 0; i < charged_ball_num; i++) {
		if (charged_ball[i]) {
			charged_ball[i].reset();
		}
	}
	operate.reset();
}

void GameBaseSceneForTest::HitConbine() {
	//プレイヤーと動かないボール間について
	for (int i = 0; i < player_num; i++) {
		for (int j = 0; j < size_up_ball_num; j++) {
			if (player[i] && size_up_ball[j]) {
				if (HitChecker_PlayerandNonMovableBall(player[i], size_up_ball[j])) {
					//運動量保存則
					double m = player[i]->Return_density() * player[i]->Return_volume() + size_up_ball[j]->Return_density() * size_up_ball[j]->Return_volume();
					player[i]->Decide_speed_x(player[i]->Return_density() * player[i]->Return_volume() * player[i]->Return_speed_x() / m);
					player[i]->Decide_speed_y(player[i]->Return_density() * player[i]->Return_volume() * player[i]->Return_speed_y() / m);

					//プレイヤー側に加算
					//密度計算
					player[i]->Decide_density((player[i]->Return_density() * player[i]->Return_volume() + size_up_ball[j]->Return_density() * size_up_ball[j]->Return_volume()) / (player[i]->Return_volume() + size_up_ball[j]->Return_volume()));
					player[i]->Add_volume(size_up_ball[j]->Return_volume());
					size_up_ball[j].reset();
				}
			}
		}
	}

	//プレイヤーと可動な電気を帯びたボール間について
	for (int i = 0; i < player_num; i++) {
		for (int j = 0; j < charged_ball_num; j++) {
			if (player[i] && charged_ball[j]) {
				if (HitChecker_PlayerandMovableChargedBall(player[i], charged_ball[j])) {
					//運動量保存則
					double m = player[i]->Return_density() * player[i]->Return_volume() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume();
					player[i]->Decide_speed_x((player[i]->Return_density() * player[i]->Return_volume() * player[i]->Return_speed_x() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume() * charged_ball[j]->Return_speed_x()) / m);
					player[i]->Decide_speed_y((player[i]->Return_density() * player[i]->Return_volume() * player[i]->Return_speed_y() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume() * charged_ball[j]->Return_speed_y()) / m);

					//プレイヤー側に加算
					//密度計算
					player[i]->Decide_density((player[i]->Return_density() * player[i]->Return_volume() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume()) / (player[i]->Return_volume() + charged_ball[j]->Return_volume()));
					player[i]->Add_volume(charged_ball[j]->Return_volume());
					player[i]->Add_charge(charged_ball[j]->Return_charge());
					player[i]->Make_TGHandle();
					charged_ball[j].reset();
				}
			}
		}
	}

	//可動な電気を帯びたボールと動かないボール間について
	for (int i = 0; i < charged_ball_num; i++) {
		for (int j = 0; j < size_up_ball_num; j++) {
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
	for (int i = 0; i < charged_ball_num; i++) {
		for (int j = i + 1; j < charged_ball_num; j++) {
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

void GameBaseSceneForTest::Gravity() {
	//初期化
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			player[i]->Decide_force_x(0.0);
			player[i]->Decide_force_y(0.0);
		}
	}
	for (int i = 0; i < charged_ball_num; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Decide_force_x(0.0);
			charged_ball[i]->Decide_force_y(0.0);
		}
	}

	//プレイヤー・電荷を帯びた可動なボール間のクーロン力
	for (int i = 0; i < player_num; i++) {
		for (int j = 0; j < charged_ball_num; j++) {
			if (player[i] && charged_ball[j]) {
				double r = pow((charged_ball[j]->Return_position_x() - player[i]->Return_position_x()) * (charged_ball[j]->Return_position_x() - player[i]->Return_position_x()) + (charged_ball[j]->Return_position_y() - player[i]->Return_position_y()) * (charged_ball[j]->Return_position_y() - player[i]->Return_position_y()), 1.0 / 2);
				double temp_x, temp_y;
				temp_x = -COULOMB_CONSTANT * player[i]->Return_charge() * charged_ball[j]->Return_charge() * (charged_ball[j]->Return_position_x() - player[i]->Return_position_x()) / (r * r * r);
				temp_y = -COULOMB_CONSTANT * player[i]->Return_charge() * charged_ball[j]->Return_charge() * (charged_ball[j]->Return_position_y() - player[i]->Return_position_y()) / (r * r * r);
				player[i]->Add_force_x(temp_x);
				player[i]->Add_force_y(temp_y);
				charged_ball[j]->Add_force_x(-temp_x);
				charged_ball[j]->Add_force_y(-temp_y);
			}
		}
	}

	//電荷を帯びた可動なボール同士のクーロン力
	for (int i = 0; i < charged_ball_num; i++) {
		for (int j = i + 1; j < charged_ball_num; j++) {
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

void GameBaseSceneForTest::Update() {
	operate->Update();			//操作処理
	TimeControl();				//時間処理
	Gravity();					//クーロン力処理
	AirResistance();			//媒質抵抗処理
	if (time_advances) {		//時が止まっているときは以下は更新しない
		for (int i = 0; i < player_num; i++) {
			if (player[i]) {
				player[i]->Update();
			}
		}
		for (int i = 0; i < charged_ball_num; i++) {
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

void GameBaseSceneForTest::Draw()const {
	for (int i = 0; i < size_up_ball_num; i++) {
		if (size_up_ball[i]) {
			size_up_ball[i]->Draw();
		}
	}
	for (int i = 0; i < charged_ball_num; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Draw();
		}
	}
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			player[i]->Draw();
		}
	}
}

bool GameBaseSceneForTest::HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player> _player, std::shared_ptr<NonMovableBall> _size_up_ball) {
	//if(当たっているならば)
	if ((_player->Return_position_x() - _size_up_ball->Return_position_x()) * (_player->Return_position_x() - _size_up_ball->Return_position_x()) + (_player->Return_position_y() - _size_up_ball->Return_position_y()) * (_player->Return_position_y() - _size_up_ball->Return_position_y()) < (_player->Return_radius() + _size_up_ball->Return_radius()) * (_player->Return_radius() + _size_up_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool GameBaseSceneForTest::HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player> _player, std::shared_ptr<MovableChargedBall> _charged_ball) {
	//if(当たっているならば)
	if ((_player->Return_position_x() - _charged_ball->Return_position_x()) * (_player->Return_position_x() - _charged_ball->Return_position_x()) + (_player->Return_position_y() - _charged_ball->Return_position_y()) * (_player->Return_position_y() - _charged_ball->Return_position_y()) < (_player->Return_radius() + _charged_ball->Return_radius()) * (_player->Return_radius() + _charged_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool GameBaseSceneForTest::HitChecker_MovableChargedBallandNonMovableBall(std::shared_ptr<MovableChargedBall> _charged_ball, std::shared_ptr<NonMovableBall> _size_up_ball) {
	//if(当たっているならば)
	if ((_charged_ball->Return_position_x() - _size_up_ball->Return_position_x()) * (_charged_ball->Return_position_x() - _size_up_ball->Return_position_x()) + (_charged_ball->Return_position_y() - _size_up_ball->Return_position_y()) * (_charged_ball->Return_position_y() - _size_up_ball->Return_position_y()) < (_charged_ball->Return_radius() + _size_up_ball->Return_radius()) * (_charged_ball->Return_radius() + _size_up_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool GameBaseSceneForTest::ClearChecker() {
	bool check = false;
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			if (player[i]->Return_charge() <= -5) {
				check = true;
				break;
			}
		}
	}
	if (check) {
		return true;
	}
	else {
		return false;
	}
}

void GameBaseSceneForTest::AirResistance() {
	//プレイヤーについて
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			player[i]->Add_force_x(-air_resistance_coefficient * player[i]->Return_speed_x());
			player[i]->Add_force_y(-air_resistance_coefficient * player[i]->Return_speed_y());
		}
	}

	//電荷を帯びた可動なボールについて
	for (int i = 0; i < charged_ball_num; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Add_force_x(-air_resistance_coefficient * charged_ball[i]->Return_speed_x());
			charged_ball[i]->Add_force_y(-air_resistance_coefficient * charged_ball[i]->Return_speed_y());
		}
	}
}

void GameBaseSceneForTest::TimeControl() {
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
		for (int i = 0; i < player_num; i++) {
			player[i]->Shoot_Operation();
			if (player[i]->Return_checker_when_time_stopped()) {
				time_advances = true;
			}
		}
	}
}

void GameBaseSceneForTest::GameClear() {
	int star, num;								//3:星3つ, 2:星2つ, 1:星1つ
	num = 5;
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			num = player[i]->Return_shoot_num();
			break;
		}
	}
	if (num <= 2) {
		star = 3;
	}
	else if (num <= 4) {
		star = 2;
	}
	else {
		star = 1;
	}
	nextScene = make_shared<GameClearScene>(star);
}

void GameBaseSceneForTest::ReloadFunction(void) {
	ReloadFileGraphAll();						// ファイルから読み込んだ画像を復元する

	//MakeScreenのグラフィックハンドルを復元
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			player[i]->Make_TGHandle();
		}
	}
	for (int i = 0; i < charged_ball_num; i++) {
		if (charged_ball[i]) {
			charged_ball[i]->Make_TGHandle();
		}
	}
}
#include "GameBaseScene.h"
#include "GameClearScene.h"

extern SceneBase* Scene_pointer_for_Reload;
extern int note_pageGHandle, page1_turnoverGHandle, pagemany_turnoverGHandle, reverse_page1_turnoverGHandle, reverse_pagemany_turnoverGHandle;
extern int page_1turnoverSH, page_manyturnoverSH, page_1turnover_reverseSH, page_manyturnover_reverseSH;
extern int makibaTH_S64_T7;
int accel_chargeSH, accel_downSH, player_shotSH;		//ゲームシーン時に使うサウンドハンドル

GameBaseScene::GameBaseScene() {
	phase = 0;
	star = 0;
	fade_in = 0;
	fade_in_speed = 17;
	fade_out = 255;
	fade_out_speed = 17;
	time_advances = false;
	operate = std::make_shared<OperationInGame>();
	Scene_pointer_for_Reload = this;
	accel_arrowGHandle = LoadGraph("graph\\arrow\\V_arrow_red.png");
	playerGHandle = LoadGraph("red_circle.png");
	charged_ballGHandle = LoadGraph("black_circle.png");
	accel_chargeSH = LoadSoundMem("sound\\player\\accel_charge.ogg");
	accel_downSH = LoadSoundMem("sound\\player\\accel_down.ogg");
	player_shotSH= LoadSoundMem("sound\\player\\shot_2.ogg");
	combineSH = LoadSoundMem("sound\\water.ogg");
}

GameBaseScene::~GameBaseScene() {
	DeleteSoundMem(accel_chargeSH);
	DeleteSoundMem(accel_downSH);
	DeleteSoundMem(player_shotSH);
	DeleteSoundMem(combineSH);
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
	DeleteGraph(accel_arrowGHandle);
}

void GameBaseScene::HitConbine() {
	//プレイヤー同士について
	for (int i = 0; i < player_num; i++) {
		for (int j = i + 1; j < player_num; j++) {
			if (player[i] && player[j]) {
				if (HitChecker_PlayerandPlayer(player[i], player[j])) {
					PlaySoundMem(combineSH, DX_PLAYTYPE_BACK, TRUE);
					if (player[i]->Return_volume() >= player[j]->Return_volume()) {				//でかい方の位置を合体後のボールの位置とする(ただし等しいならば真ん中)
						//運動量保存則
						double m = player[i]->Return_density() * player[i]->Return_volume() + player[j]->Return_density() * player[j]->Return_volume();
						player[i]->Decide_speed_x((player[i]->Return_density() * player[i]->Return_volume() * player[i]->Return_speed_x() + player[j]->Return_density() * player[j]->Return_volume() * player[j]->Return_speed_x()) / m);
						player[i]->Decide_speed_y((player[i]->Return_density() * player[i]->Return_volume() * player[i]->Return_speed_y() + player[j]->Return_density() * player[j]->Return_volume() * player[j]->Return_speed_y()) / m);

						//等しいならば真ん中を中心座標にする
						if (player[i]->Return_volume() == player[j]->Return_volume()) {
							player[i]->Decide_position_x((player[i]->Return_position_x() + player[j]->Return_position_x()) / 2);
							player[i]->Decide_position_y((player[i]->Return_position_y() + player[j]->Return_position_y()) / 2);
						}

						//プレイヤー側に加算
						//密度計算
						player[i]->Decide_density((player[i]->Return_density() * player[i]->Return_volume() + player[j]->Return_density() * player[j]->Return_volume()) / (player[i]->Return_volume() + player[j]->Return_volume()));
						player[i]->Add_volume(player[j]->Return_volume());
						player[i]->Add_charge(player[j]->Return_charge());
						player[i]->Make_TGHandle();
						player[j].reset();
					}
					else {
						//運動量保存則
						double m = player[j]->Return_density() * player[j]->Return_volume() + player[i]->Return_density() * player[i]->Return_volume();
						player[j]->Decide_speed_x((player[i]->Return_density() * player[i]->Return_volume() * player[i]->Return_speed_x() + player[j]->Return_density() * player[j]->Return_volume() * player[j]->Return_speed_x()) / m);
						player[j]->Decide_speed_y((player[i]->Return_density() * player[i]->Return_volume() * player[i]->Return_speed_y() + player[j]->Return_density() * player[j]->Return_volume() * player[j]->Return_speed_y()) / m);

						//プレイヤー側に加算
						//密度計算
						player[j]->Decide_density((player[i]->Return_density() * player[i]->Return_volume() + player[j]->Return_density() * player[j]->Return_volume()) / (player[i]->Return_volume() + player[j]->Return_volume()));
						player[j]->Add_volume(player[i]->Return_volume());
						player[j]->Add_charge(player[i]->Return_charge());
						player[j]->Make_TGHandle();
						player[i].reset();
					}
				}
			}
		}
	}

	//プレイヤーと動かないボール間について
	for (int i = 0; i < player_num; i++) {
		for (int j = 0; j < size_up_ball_num; j++) {
			if (player[i] && size_up_ball[j]) {
				if (HitChecker_PlayerandNonMovableBall(player[i], size_up_ball[j])) {
					PlaySoundMem(combineSH, DX_PLAYTYPE_BACK, TRUE);
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
	for (int i = 0; i < player_num; i++){
		for (int j = 0; j < charged_ball_num; j++) {
			if (player[i] && charged_ball[j]) {
				if (HitChecker_PlayerandMovableChargedBall(player[i], charged_ball[j])) {
					PlaySoundMem(combineSH, DX_PLAYTYPE_BACK, TRUE);
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
					PlaySoundMem(combineSH, DX_PLAYTYPE_BACK, TRUE);
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
					PlaySoundMem(combineSH, DX_PLAYTYPE_BACK, TRUE);
					if (charged_ball[i]->Return_volume() >= charged_ball[j]->Return_volume()) {				//でかい方の位置を合体後のボールの位置とする(ただし等しいならば真ん中)
						//運動量保存則
						double m = charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume();
						charged_ball[i]->Decide_speed_x((charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_x() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume() * charged_ball[j]->Return_speed_x()) / m);
						charged_ball[i]->Decide_speed_y((charged_ball[i]->Return_density() * charged_ball[i]->Return_volume() * charged_ball[i]->Return_speed_y() + charged_ball[j]->Return_density() * charged_ball[j]->Return_volume() * charged_ball[j]->Return_speed_y()) / m);

						//等しいならば真ん中を中心座標にする
						if (charged_ball[i]->Return_volume() == charged_ball[j]->Return_volume()) {
							charged_ball[i]->Decide_position_x((charged_ball[i]->Return_position_x() + charged_ball[j]->Return_position_x()) / 2);
							charged_ball[i]->Decide_position_y((charged_ball[i]->Return_position_y() + charged_ball[j]->Return_position_y()) / 2);
						}

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

void GameBaseScene::Gravity() {
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

	//プレイヤー同士のクーロン力
	for (int i = 0; i < player_num; i++) {
		for (int j = i + 1; j < player_num; j++) {
			if (player[i] && player[j]) {
				double r = pow((player[i]->Return_position_x() - player[j]->Return_position_x()) * (player[i]->Return_position_x() - player[j]->Return_position_x()) + (player[i]->Return_position_y() - player[j]->Return_position_y()) * (player[i]->Return_position_y() - player[j]->Return_position_y()), 1.0 / 2);
				double temp_x, temp_y;
				temp_x = -COULOMB_CONSTANT * player[j]->Return_charge() * player[i]->Return_charge() * (player[i]->Return_position_x() - player[j]->Return_position_x()) / (r * r * r);
				temp_y = -COULOMB_CONSTANT * player[j]->Return_charge() * player[i]->Return_charge() * (player[i]->Return_position_y() - player[j]->Return_position_y()) / (r * r * r);
				player[j]->Add_force_x(temp_x);
				player[j]->Add_force_y(temp_y);
				player[i]->Add_force_x(-temp_x);
				player[i]->Add_force_y(-temp_y);
			}
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

void GameBaseScene::Update() {
	if (phase == 0) {
		if (fade_in < 255) {
			fade_in += fade_in_speed;
		}
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
	//GameClearによってphage==1となった直後に、↓のif分内でphage==2となるようにした
	if (phase == 1) {
		SetAlwaysRunFlag(TRUE);
		PlaySoundMem(page_1turnoverSH, DX_PLAYTYPE_BACK, TRUE);
		PlayMovieToGraph(page1_turnoverGHandle);
		phase = 2;
	}
	else if (phase == 2) {
		if (fade_out > 0) {
			fade_out -= fade_out_speed;
		}
		if (GetMovieStateToGraph(page1_turnoverGHandle) == 0) {
			SetAlwaysRunFlag(FALSE);
			SeekMovieToGraph(page1_turnoverGHandle, 0);
			nextScene = std::make_shared<GameClearScene>(star);
		}
	}
}

void GameBaseScene::Draw()const {
	if (phase == 0) {
		DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
		if (fade_in < 255) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_in);
			Draw_Objects();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			Draw_Objects();
		}
	}
	else if (phase == 2) {
		if (GetMovieStateToGraph(page1_turnoverGHandle) == 1) {
			DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0, 1.5, 0, page1_turnoverGHandle, TRUE, FALSE);
		}
		else {
			DrawRotaGraph(ANDROID_WIDTH / 2.0, ANDROID_HEIGHT / 2.0 + 6, 1.02, 0, note_pageGHandle, TRUE, FALSE);		//背景
		}
		if (fade_out > 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_out);
			Draw_Objects();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void GameBaseScene::Draw_Objects()const {
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
	Draw_Purpose();
}

bool GameBaseScene::HitChecker_PlayerandPlayer(std::shared_ptr<Player> _player1, std::shared_ptr<Player> _player2) {
	//if(当たっているならば)
	if ((_player1->Return_position_x() - _player2->Return_position_x()) * (_player1->Return_position_x() - _player2->Return_position_x()) + (_player1->Return_position_y() - _player2->Return_position_y()) * (_player1->Return_position_y() - _player2->Return_position_y()) < (_player1->Return_radius() + _player2->Return_radius()) * (_player1->Return_radius() + _player2->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool GameBaseScene::HitChecker_PlayerandNonMovableBall(std::shared_ptr<Player> _player, std::shared_ptr<NonMovableBall> _size_up_ball) {
	//if(当たっているならば)
	if ((_player->Return_position_x() - _size_up_ball->Return_position_x()) * (_player->Return_position_x() - _size_up_ball->Return_position_x()) + (_player->Return_position_y() - _size_up_ball->Return_position_y()) * (_player->Return_position_y() - _size_up_ball->Return_position_y()) < (_player->Return_radius() + _size_up_ball->Return_radius()) * (_player->Return_radius() + _size_up_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool GameBaseScene::HitChecker_PlayerandMovableChargedBall(std::shared_ptr<Player> _player, std::shared_ptr<MovableChargedBall> _charged_ball) {
	//if(当たっているならば)
	if ((_player->Return_position_x() - _charged_ball->Return_position_x()) * (_player->Return_position_x() - _charged_ball->Return_position_x()) + (_player->Return_position_y() - _charged_ball->Return_position_y()) * (_player->Return_position_y() - _charged_ball->Return_position_y()) < (_player->Return_radius() + _charged_ball->Return_radius()) * (_player->Return_radius() + _charged_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

bool GameBaseScene::HitChecker_MovableChargedBallandNonMovableBall(std::shared_ptr<MovableChargedBall> _charged_ball, std::shared_ptr<NonMovableBall> _size_up_ball) {
	//if(当たっているならば)
	if ((_charged_ball->Return_position_x() - _size_up_ball->Return_position_x()) * (_charged_ball->Return_position_x() - _size_up_ball->Return_position_x()) + (_charged_ball->Return_position_y() - _size_up_ball->Return_position_y()) * (_charged_ball->Return_position_y() - _size_up_ball->Return_position_y()) < (_charged_ball->Return_radius() + _size_up_ball->Return_radius()) * (_charged_ball->Return_radius() + _size_up_ball->Return_radius())) {
		return true;
	}
	else {
		return false;
	}
}

void GameBaseScene::AirResistance() {
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

void GameBaseScene::TimeControl() {
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
			if (player[i]) {
				player[i]->Shoot_Operation();
				if (player[i]->Return_checker_when_time_stopped()) {
					time_advances = true;
				}
			}
		}
	}
}

void GameBaseScene::ReloadFunction(void) {
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

void GameBaseScene::Draw_Purpose()const {
	DrawFormatStringToHandle(100, 0, GetColor(0, 0, 0), makibaTH_S64_T7, "%s", purpose.c_str());
	for (int i = 0; i < player_num; i++) {
		if (player[i]) {
			DrawFormatStringToHandle(100, 100, GetColor(0, 0, 0), makibaTH_S64_T7, "%d回", player[i]->Return_shoot_num());
			break;
		}
	}
}
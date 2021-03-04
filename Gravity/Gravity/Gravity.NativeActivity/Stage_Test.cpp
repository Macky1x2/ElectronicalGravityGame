//テスト開発用
#include "Stage_Test.h"

Stage_Test::Stage_Test() {
	air_resistance_coefficient = 0.01;
	player_num = 1;
	size_up_ball_num = 3;
	charged_ball_num = 3;
	player = new std::shared_ptr<Player>[player_num];
	size_up_ball = new std::shared_ptr<NonMovableBall>[size_up_ball_num];
	charged_ball = new std::shared_ptr<MovableChargedBall>[charged_ball_num];
	player[0] = std::make_shared<Player>(500, 500, 5, 8, 0.5);						//引数(初期x座標, 初期y座標, 電荷, 体積, 密度)//初期座標は別ファイルから読み取るのがいいかもしれない
	size_up_ball[0] = std::make_shared<NonMovableBall>(300, 1000, 3, 0.5);				//引数(初期x座標, 初期y座標, 体積, 密度)
	size_up_ball[1] = std::make_shared<NonMovableBall>(600, 1500, 3, 0.5);
	size_up_ball[2] = std::make_shared<NonMovableBall>(1050, 300, 30, 0.5);
	charged_ball[0] = std::make_shared<MovableChargedBall>(750, 400, -5, 8, 0.5);			//引数(初期x座標, 初期y座標, 電荷, 体積, 密度)
	charged_ball[1] = std::make_shared<MovableChargedBall>(100, 1100, -5, 8, 0.5);
	charged_ball[2] = std::make_shared<MovableChargedBall>(250, 1700, -5, 8, 0.5);
}

Stage_Test::~Stage_Test() {

}
#include "GameScene.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {

}

void GameScene::Update() {

}

void GameScene::Draw()const {
	DrawLine(500, 0, 600, 1080, GetColor(255, 255, 255));
}
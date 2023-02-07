#include"Main.h"
#include"GameScene.h"
#include"Field.h"
#include"Player.h"
#include"PlayerUI.h"
#include"SceneManager.h"

GameScene::GameScene() {
	AddGameObject<Field>();
	AddGameObject<Player>()->SetPosition(XMFLOAT3(0, 0, -5));

	Player* player = GetGameObject<Player>();
	AddGameObject<PlayerUI>(Layer::SPRITE)->SetPlayer(player);

}
void GameScene::Update() {
	Scene::Update();
	ChangeScene();
}
void GameScene::DrawGeometry() {
	Scene::DrawGeometry();
}
void GameScene::DrawSprite() {
	Scene::DrawSprite();
}
void GameScene::Finalize() {
	Scene::Finalize();
}


void GameScene::ChangeScene() {

}
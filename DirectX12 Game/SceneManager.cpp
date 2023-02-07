#include"SceneManager.h"
#include"Camera.h"
#include"GameScene.h"
#include"Renderer.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager() {
	isGameEnd = false;
	Camera::Create();
	m_scene = new GameScene();
}
SceneManager::~SceneManager() {
	Camera::Destroy();
	m_scene->Finalize();
	delete m_scene;

}

void SceneManager::Update() {
	m_scene->Update();
	Camera::GetInstance()->Update();
}
void SceneManager::Draw() {
	Camera::GetInstance()->Draw();
	//シャドウ用深度を書き込む用
	Renderer::GetInstance()->ShadowPassStart();
	m_scene->DrawGeometry();
	Renderer::GetInstance()->ShadowPassEnd();

	//ジオメトリの色んな情報を書き込む用
	Renderer::GetInstance()->GeometryPassStart();
	m_scene->DrawGeometry();
	Renderer::GetInstance()->GeometryPassEnd();



	//最終描画
	Renderer::GetInstance()->Draw2DStart();
	m_scene->DrawSprite();
	Renderer::GetInstance()->Draw2DEnd();

	Renderer::GetInstance()->DrawEnd();
}
Scene* SceneManager::GetScene() {
	return m_scene;
}
void SceneManager::GameEnd() {
	isGameEnd = true;
}
bool SceneManager::GetIsGameEnd() {
	return isGameEnd;
}


void SceneManager::Create() {
	if (!instance) {
		instance = new SceneManager();
	}
}
void SceneManager::Destroy() {
	delete instance;
	instance = nullptr;
}
SceneManager* SceneManager::GetInstance() {
	return instance;
}


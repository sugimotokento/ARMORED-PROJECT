#include"SceneManager.h"
#include"Scene.h"
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
	//�V���h�E�p�[�x���������ޗp
	Renderer::GetInstance()->ShadowPassStart();
	m_scene->Draw(Scene::Layer::GEOMETRY);
	m_scene->Draw(Scene::Layer::WATER);
	m_scene->Draw(Scene::Layer::ALPHA);
	Renderer::GetInstance()->ShadowPassEnd();

	//�W�I���g���̐F��ȏ����������ޗp
	Renderer::GetInstance()->GeometryPassStart();
	m_scene->Draw(Scene::Layer::GEOMETRY);
	m_scene->Draw(Scene::Layer::ALPHA);
	Renderer::GetInstance()->GeometryPassEnd();

	////�W�I���g���̐F��ȏ����������ޗp
	Renderer::GetInstance()->GeometryAlphaPassStart();
	m_scene->Draw(Scene::Layer::GEOMETRY);
	m_scene->Draw(Scene::Layer::WATER);
	m_scene->Draw(Scene::Layer::ALPHA);
	Renderer::GetInstance()->GeometryAlphaPassEnd();


	//�ŏI�`��
	Renderer::GetInstance()->Draw2DStart();
	m_scene->Draw(Scene::Layer::SPRITE);
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


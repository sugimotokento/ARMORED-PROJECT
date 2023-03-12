#include"SceneManager.h"
#include"Scene.h"
#include"CameraManager.h"
#include"GameScene.h"
#include"Renderer.h"


#ifdef _DEBUG
#include"DebugMenuScene.h"
#endif // _DEBUG

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager() {
	isGameEnd = false;
	CameraManager::Create();
#ifdef _DEBUG
	m_scene = new DebugMenuScene();
#else 
	m_scene = new GameScene();
#endif
	m_scene->Initialize();
}
SceneManager::~SceneManager() {
	CameraManager::Destroy();
	m_scene->Finalize();
	delete m_scene;

}

void SceneManager::Update() {
	if (!m_scene)return;
	m_scene->Update();
	CameraManager::GetInstance()->Update();
}
void SceneManager::Draw() {
	if (!m_scene)return;
	CameraManager::GetInstance()->Draw();
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

	////���߃I�u�W�F�N�g��Color����������
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


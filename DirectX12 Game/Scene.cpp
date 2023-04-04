#include"Scene.h"
#include"GameObject.h"
#include<stdio.h>
#include"Call.h"
#include"Renderer.h"
#include"ImguiRenderer.h"

void Scene::Initialize() {

}

void Scene::Update() {
	for (int i = 0; i < Layer::COUNT; i++) {
		for (GameObject* obj : m_gameObject[i]) {
			obj->Update();
		}
	}
	Call::GetInstance()->DelayUpdate();

	for (int i = 0; i < Layer::COUNT; i++) {
		m_gameObject[i].remove_if([](GameObject* obj) {
			return obj->Destroy();//Destroy()‚Ì’†‚ÅFinalize‚ðŒÄ‚ñ‚Å‚¢‚é
			});
	}
}

void Scene::Draw(Layer layer) {
	for (GameObject* obj : m_gameObject[layer]) {
		obj->Draw();
	}
}


void Scene::Finalize() {
	for (int i = 0; i < Layer::COUNT; i++) {
		for (GameObject* obj : m_gameObject[i]) {
			obj->Finalize();
			delete obj;

		}
		m_gameObject[i].clear();
	}
}



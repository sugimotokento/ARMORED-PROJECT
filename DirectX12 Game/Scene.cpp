#include"Scene.h"
#include"GameObject.h"
#include<stdio.h>
#include"Call.h"


void Scene::Update() {
	for (int i = 0; i < Layer::COUNT; i++) {
		for (GameObject* obj : m_gameObject[i]) {
			obj->Update();
		}
	}
	Call::GetInstance()->DelayUpdate();

	for (int i = 0; i < Layer::COUNT; i++) {
		m_gameObject[i].remove_if([](GameObject* obj) {
			return obj->Destroy();
			});
	}
}

void Scene::DrawGeometry() {
	for (GameObject* obj : m_gameObject[Layer::GEOMETRY]) {
		obj->Draw();
	}
}
void Scene::DrawSprite() {
	for (GameObject* obj : m_gameObject[Layer::SPRITE]) {
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



#pragma once
#include"Scene.h"
#include<stdio.h>
#include<conio.h>

class SceneManager {
private:
	Scene* m_scene;
	bool isGameEnd;

	static SceneManager* instance;

	SceneManager();
public:
	~SceneManager();
	void Update();
	void Draw();
	void GameEnd();
	Scene* GetScene();
	bool GetIsGameEnd();

	static void Create();
	static void Destroy();
	static SceneManager* GetInstance();

	template <typename T>
	void SetScene() {
		if (m_scene) {
			m_scene->Finalize();
			delete m_scene;
			m_scene = nullptr;
		}

		T* scene = new T();
		m_scene = scene;
		m_scene->Initialize();
	}

};
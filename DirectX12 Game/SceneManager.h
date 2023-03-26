#pragma once
#include"Scene.h"
#include<stdio.h>
#include<conio.h>
#include"ModelLoader.h"

class LoadUI;

class SceneManager {
private:
	Scene* m_scene;
	LoadUI* m_loadUI;
	bool isGameEnd;

	static SceneManager* instance;

	SceneManager();

	bool m_isSceneLoadEnd = true;
	void LoadScene();
	void DrawLoadAnimation();
	
public:
	~SceneManager();
	void Update();
	void Draw();
	void InitializeScene();
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
			ModelLoader::GetInstance()->Finalize();
			delete m_scene;
			m_scene = nullptr;
		}
		m_isSceneLoadEnd = false;
		T* scene = new T();
		m_scene = scene;
		//InitializeScene();
	}

};
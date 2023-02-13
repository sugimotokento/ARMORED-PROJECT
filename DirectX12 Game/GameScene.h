#pragma once
#include"Scene.h"

class GameScene :public Scene {
private:
	const int CHANGE_SCENE_INTERVAL = 150;
	int m_intervalCount;
	void ChangeScene();
public:
	GameScene();
	~GameScene() {}
	void Update()final override;
	void Draw(Layer layer)final override;
	void Finalize()final override;
};
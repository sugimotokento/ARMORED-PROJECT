#pragma once
#include"Scene.h"

class DebugBattleScene : public Scene {
public:
	DebugBattleScene();
	~DebugBattleScene() {}
	void Initialize()final override;
	void Update()final override;
	void Draw(Layer layer)final override;
	void Finalize()final override;
};
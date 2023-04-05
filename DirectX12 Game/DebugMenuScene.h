#pragma once
#ifdef _DEBUG
#include"Scene.h"

class DebugMenuScene :public Scene {
private:
	bool ImguiDebug(bool isVisible);

public:
	DebugMenuScene();
	~DebugMenuScene() {}
	void Initialize()final override;
	void Update()final override;
	void Draw(Layer layer)final override;
	void Finalize()final override;
};

#endif
#pragma once
#ifdef _DEBUG

#include"Scene.h"

class DebugWeaponViewerScene :public Scene {
public:
	DebugWeaponViewerScene();
	~DebugWeaponViewerScene() {}
	void Initialize()final override;
	void Update()final override;
	void Draw(Layer layer)final override;
	void Finalize()final override;
};


#endif
#pragma once
#ifdef _DEBUG
#include"GameObject.h"

class DebugBattleMenu :public GameObject{
public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	bool ImguiDebug();
};
#endif
#pragma once
#include"GameObject.h"

class Sprite;
class LoadUI :public GameObject {
private:
	Sprite* m_rotSprite;
	Sprite* m_backGround;
public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};
#pragma once
#include"GameObject.h"
#include<list>

class Sprite;
class Texture;
class Player;
class PlayerUI :public GameObject {
private:
	Sprite* m_rotSprite;

#ifdef _DEBUG
	bool ImguiDebug(bool isVisible);
#endif // _DEBUG
public:
	PlayerUI();
	void Initialize() final override {}
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void SetPlayer(Player* player);
};
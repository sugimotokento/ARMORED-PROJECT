#pragma once
#include"GameObject.h"
#include<list>

class Sprite;
class Texture;
class Player;
class PlayerUI :public GameObject {
private:
	Player* m_player;
	std::list<Sprite*> m_stock;
	Sprite* m_frame;

public:
	PlayerUI();
	void Initialize() final override {}
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void SetPlayer(Player* player);
};
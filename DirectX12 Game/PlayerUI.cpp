#include"PlayerUI.h"
#include"Sprite.h"
#include"Texture.h"
#include"Player.h"

PlayerUI::PlayerUI() {
	m_player = nullptr;


}
void PlayerUI::Update() {

}
void PlayerUI::Draw() {	
	m_frame->Draw();

	int i = 0;
	for (Sprite* sprite : m_stock) {
		if (i < m_player->GetHP()) {
			sprite->Draw();
		}
		i++;
	}
	
}
void PlayerUI::Finalize() {
	for (Sprite* sprite : m_stock) {
		sprite->Finalize();
		delete sprite;
	}
	m_stock.clear();


	m_frame->Finalize();
	delete m_frame;
}


void PlayerUI::SetPlayer(Player* player) {
	m_player = player;

	float x = 75;
	float y = 75;
	for (int i = 0; i < m_player->MAX_HP; i++) {
		Sprite* stock = new Sprite(L"asset/playerHP.dds");

		stock->SetPosition(XMFLOAT3(x+60*i, y, 0));
		stock->SetScale(XMFLOAT3(50, 50, 1));
		stock->SetColor(XMFLOAT4(0, 0, 1, 1));

		m_stock.push_back(stock);
	}
	

	m_frame = new Sprite(L"asset/HPFrame.dds");

	m_frame->SetPosition(XMFLOAT3(200, 75, 0));
	m_frame->SetScale(XMFLOAT3(350, 150, 1));
	m_frame->SetColor(XMFLOAT4(1, 1, 1, 1));
}
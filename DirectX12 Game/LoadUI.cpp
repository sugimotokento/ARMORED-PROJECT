#include"LoadUI.h"
#include"Sprite.h"

void LoadUI::Initialize() {
	m_rotSprite = new Sprite(L"asset/Texture/White.png");
	m_rotSprite->SetPosition(XMFLOAT3(SCREEN_WIDTH-130, SCREEN_HEIGHT-150, 0));
	m_rotSprite->SetScale(XMFLOAT3(100, 100, 100));

	m_backGround = new Sprite(L"asset/Texture/UI/Loading/NowLoading.png");
	m_backGround->SetPosition(XMFLOAT3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-50, 0));
	m_backGround->SetScale(XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));
}
void LoadUI::Update() {
	static float rot = 0;
	rot += 0.01f;

	m_rotSprite->SetRotation(XMFLOAT3(0, 0, rot));
}
void LoadUI::Draw() {
	m_backGround->Draw();
	m_rotSprite->Draw();
}
void LoadUI::Finalize() {
	m_backGround->Finalize();
	m_rotSprite->Finalize();
	delete m_backGround;
	delete m_rotSprite;
}
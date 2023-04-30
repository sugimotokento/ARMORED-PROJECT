#include"PlayerUI.h"
#include"Sprite.h"
#include"Texture.h"
#include"Player.h"
#include"ImguiRenderer.h"
#include<math.h>

PlayerUI::PlayerUI() {
	m_rotSprite = new Sprite(L"asset/Texture/White.png");
	m_rotSprite->SetPosition(XMFLOAT3(SCREEN_WIDTH - 130, SCREEN_HEIGHT - 150, 0));
	m_rotSprite->SetScale(XMFLOAT3(100, 100, 100));

#ifdef _DEBUG
	std::function<bool(bool isVisible)> f = std::bind(&PlayerUI::ImguiDebug, this, std::placeholders::_1);
	ImguiRenderer::GetInstance()->AddFunction(f, "PlayerUI", true);
#endif // _DEBUG
}
void PlayerUI::Update() {

}
void PlayerUI::Draw() {	
	m_rotSprite->Draw();
}
void PlayerUI::Finalize() {
	m_rotSprite->Finalize();
	delete m_rotSprite;
}


void PlayerUI::SetPlayer(Player* player) {

}

#ifdef _DEBUG
bool PlayerUI::ImguiDebug(bool isVisible) {
	if (isVisible) {
		ImGui::Begin("PlayerUI");
		static float radialRatio = 1;
		ImGui::SliderFloat("Ratio", &radialRatio, 0, 1);
	

		static float startAngle = 0;
		ImGui::SliderFloat("StartAngle", &startAngle, 0, 6.28f);

		static bool isInvers = false;
		ImGui::Checkbox("Invers", &isInvers);

		m_rotSprite->SetRadialParam(radialRatio, startAngle, isInvers);
		ImGui::End();
	}
	return GetIsDestroy();
}
#endif // _DEBUG
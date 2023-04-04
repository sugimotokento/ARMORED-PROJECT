#include"Shotgun.h"
#include"SceneManager.h"
#include"Scene.h"
#include"XMMath.h"
#include"ImguiRenderer.h"
#include"ModelLoader.h"
#include <cstdlib>

void Shotgun::Initialize() {

	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_WEAPON_SHOTGUN);

	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);

	//武器のパラメーター設定
	m_spreadRatio = 0.29f;


	//弾のパラメーター設定
	m_bulletOffset.x = 0;
	m_bulletOffset.y = 0.275f;
	m_bulletOffset.z = 0.29f;
	m_bulletSetting.range = 300;
	m_bulletSetting.speed = 50;
	m_intervalMax = 15;

	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);


#ifdef _DEBUG
	std::function<bool()> f = std::bind(&Shotgun::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f, "Shotgun");
#endif // _DEBUG
}
void Shotgun::Update() {
	m_intervalCount++;
}
void Shotgun::Draw() {
	////マトリクス設定
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();

	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	CreateWorldMTX(trans, rot, size);

	XMMATRIX world = XMLoadFloat4x4(&m_worldMTX);

	//定数バッファ設定
	ConstantBuffer* constant;
	m_constantBuffer->Map(0, nullptr, (void**)&constant);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * view * projection));
	constant->wvp = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * lightView * lightProjection));
	constant->wvpLight = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(view * projection));
	constant->vp = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world));

	constant->world = matrix;
	constant->reflectRate = XMFLOAT4(0, 0, 0, 0);
	constant->cameraPosition = CameraManager::GetInstance()->GetMainPosition();

	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());


	//モデル描画
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_WEAPON_SHOTGUN);

}
void Shotgun::Finalize() {

}

void Shotgun::Shot() {
	if (m_intervalCount > m_intervalMax) {
		for (int i = 0; i < SHOT_NUM; i++) {
			const float RANDOM_MAX = 10.0f;
			float randomX = (float)(rand() % ((int)RANDOM_MAX * 10000));
			randomX /= 10000;
			if ((rand() % 2) == 0)randomX *= -1;
			float randomY = (float)(rand() % ((int)RANDOM_MAX * 10000));
			randomY /= 10000;
			if ((rand() % 2) == 0)randomY *= -1;
			float randomZ = (float)(rand() % ((int)RANDOM_MAX * 10000));
			randomZ /= 10000;
			if ((rand() % 2) == 0)randomZ *= -1;

			m_bulletSetting.moveDir = GetForward() * RANDOM_MAX + GetRight() * randomX*m_spreadRatio + GetUp() * randomY * m_spreadRatio;
			m_bulletSetting.moveDir = XMMath::Normalize(m_bulletSetting.moveDir);
			XMFLOAT3 offset = GetRight() * m_bulletOffset.x + GetUp() * m_bulletOffset.y + GetForward() * m_bulletOffset.z;
			XMFLOAT3 worldPos = GetWorldPosition();
			m_bulletSetting.position = worldPos + offset;
			m_bulletSetting.rotation = GetRotation();

			SceneManager::GetInstance()->GetScene()->AddGameObject<Ammunition>()->SetSettingItem(m_bulletSetting);
		}
		m_intervalCount = 0;
	}
	
}


#ifdef _DEBUG
bool Shotgun::ImguiDebug() {
	ImGui::Begin("Shotgun");
	ImGui::Text("  x:%lf, y:%lf, z:%lf", m_bulletOffset.x, m_bulletOffset.y, m_bulletOffset.z);
	ImGui::SliderFloat("x", &m_bulletOffset.x, -2, 2);
	ImGui::SliderFloat("y", &m_bulletOffset.y, -2, 2);
	ImGui::SliderFloat("z", &m_bulletOffset.z, -2, 2);
	ImGui::Text("SpreadValue");
	ImGui::SliderFloat("spreadRatio", &m_spreadRatio, 0, 1);
	ImGui::End();
	return GetIsDestroy();
}
#endif // _DEBUG
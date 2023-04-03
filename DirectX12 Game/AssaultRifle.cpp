#include"AssaultRifle.h"
#include"SceneManager.h"
#include"Scene.h"
#include"XMMath.h"
#include"ImguiRenderer.h"
#include"ModelLoader.h"

void AssaultRifle::Initialize() {

	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_WEAPON_ASSAULT_RIFLE);

	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);

	

	//弾のパラメーター設定
	m_bulletOffset.x = 0;
	m_bulletOffset.y = 0.149f;
	m_bulletOffset.z = 2.0f;
	m_bulletSetting.range = 1000;
	m_bulletSetting.speed = 50;
	m_intervalMax = 3.5f;

	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);


#ifdef _DEBUG
	/*std::function<bool()> f = std::bind(&AssaultRifle::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f);*/
#endif // _DEBUG
}
void AssaultRifle::Update() {
	m_intervalCount++;
}
void AssaultRifle::Draw() {
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
	constant->cameraPosition = CameraManager::GetInstance()->GetMainPosition();
	constant->world = matrix;
	constant->reflectRate = XMFLOAT4(0, 0, 0, 0);


	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());


	//モデル描画
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_WEAPON_ASSAULT_RIFLE);

}
void AssaultRifle::Finalize() {

}

void AssaultRifle::Shot() {
	if (m_intervalCount > m_intervalMax) {
		m_bulletSetting.moveDir = GetForward();
		XMFLOAT3 offset = GetRight() * m_bulletOffset.x + GetUp() * m_bulletOffset.y + GetForward() * m_bulletOffset.z;
		XMFLOAT3 worldPos = GetWorldPosition();
		m_bulletSetting.position = worldPos + offset;
		m_bulletSetting.rotation = GetRotation();

		SceneManager::GetInstance()->GetScene()->AddGameObject<Ammunition>()->SetSettingItem(m_bulletSetting);

		m_intervalCount = 0;

	}
}


#ifdef _DEBUG
bool AssaultRifle::ImguiDebug() {
	ImGui::Begin("AssaultRifle");
	ImGui::Text("  x:%lf, y:%lf, z:%lf", m_bulletOffset.x, m_bulletOffset.y, m_bulletOffset.z);
	ImGui::SliderFloat("x", &m_bulletOffset.x, -2, 2);
	ImGui::SliderFloat("y", &m_bulletOffset.y, -2, 2);
	ImGui::SliderFloat("z", &m_bulletOffset.z, -2, 2);
	ImGui::End();
	return GetIsDestroy();
}
#endif // _DEBUG
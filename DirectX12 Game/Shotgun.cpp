#include"Shotgun.h"
#include"SceneManager.h"
#include"Scene.h"
#include"XMMath.h"
#include"ImguiRenderer.h"
#include"ModelLoader.h"

void Shotgun::Initialize() {

	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_WEAPON_SHOTGUN);

	m_scale = XMFLOAT3(0.0025f, 0.0025f, 0.0025f);
	m_rotation = XMFLOAT3(-0.1f, 0, 0);



	//弾のパラメーター設定
	m_bulletOffset.x = 0;
	m_bulletOffset.y = 0.275f;
	m_bulletOffset.z = 2.0f;
	m_bulletSetting.range = 150;
	m_bulletSetting.speed = 2;
	m_intervalMax = 10;

	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);


#ifdef _DEBUG
	std::function<bool()> f = std::bind(&Shotgun::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f);
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
	XMMATRIX world = size * rot * trans;
	XMStoreFloat4x4(&m_worldMTX, world);

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


	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());


	//モデル描画
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_WEAPON_SHOTGUN);

}
void Shotgun::Finalize() {
	m_constantBuffer.Get()->Release();
}

void Shotgun::Shot() {
	if (m_intervalCount > m_intervalMax) {
		m_bulletSetting.moveDir = GetForward();
		XMFLOAT3 offset = GetRight() * m_bulletOffset.x + GetUp() * m_bulletOffset.y + GetForward() * m_bulletOffset.z;
		m_bulletSetting.position = GetPosition() + offset;
		m_bulletSetting.rotation = GetRotation();

		SceneManager::GetInstance()->GetScene()->AddGameObject<Ammunition>()->SetSettingItem(m_bulletSetting);

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
	ImGui::End();
	return false;
}
#endif // _DEBUG
#include"Ammunition.h"
#include"XMMath.h"
#include"ImguiRenderer.h"
#include"ModelLoader.h"
#include"XMMath.h"
#include<math.h>

void Ammunition::Initialize() {
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_BULLET_AMMUNITION);
	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

	m_scale = XMFLOAT3(0.06f, 0.06f, 0.32f);

	XMVECTOR quat = XMQuaternionIdentity();
	XMStoreFloat4(&m_quaternion, quat);

#ifdef _DEBUG
	std::function<bool()> f = std::bind(&Ammunition::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f, "Ammunition");
#endif // _DEBUG
}
void Ammunition::Update() {
	m_position += m_moveDir * m_speed;


	XMFLOAT3 dist;
	dist = m_startPosition - m_position;
	if (XMMath::Length(dist) > m_range) {
		SetDestroy();
	}
}
void Ammunition::Draw() {
	////マトリクス設定
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();

	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&m_quaternion));
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
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_BULLET_AMMUNITION);

}
void Ammunition::Finalize() {
	m_constantBuffer.Get()->Release();
}

void Ammunition::SetSettingItem(SettingItem item) {
	m_moveDir = item.moveDir;
	m_position = item.position;
	m_rotation = item.rotation;
	m_speed = item.speed;
	m_range = item.range;

	m_startPosition = item.position;

	XMFLOAT3 forward = XMFLOAT3(0,0,1);
	float dot = XMMath::Dot(forward, m_moveDir);
	float angle = acosf(dot);
	if (fabsf(angle) < 0.01f)return;

	XMFLOAT3 axis = XMMath::Cross(forward, m_moveDir);
	axis = XMMath::Normalize(axis);
	m_quaternion = XMMath::QuaternionRotateAxis(m_quaternion, axis, angle);
}



#ifdef _DEBUG
bool Ammunition::ImguiDebug() {
	XMFLOAT3 dist;
	dist = m_startPosition - m_position;
	ImGui::Begin("Ammunition");
	ImGui::Text("dist:%lf,", XMMath::Length(dist));

	ImGui::End();
	return GetIsDestroy();
}
#endif // _DEBUG
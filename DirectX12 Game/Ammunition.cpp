#include"Ammunition.h"
#include"XMMath.h"
#include"ImguiRenderer.h"
#include"ModelLoader.h"

void Ammunition::Initialize() {
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_BULLET_AMMUNITION);
	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

	m_scale = XMFLOAT3(0.03f, 0.03f, 0.16f);


#ifdef _DEBUG
	//std::function<bool()> f = std::bind(&Ammunition::ImguiDebug, this);
	//ImguiRenderer::GetInstance()->AddFunction(f);
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
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_BULLET_AMMUNITION);

}
void Ammunition::Finalize() {
	m_constantBuffer.Get()->Release();
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
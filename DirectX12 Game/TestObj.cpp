#include"Model.h"

#include"TestObj.h"

#include"TextureGeometry.h"
#include"Scene.h"
#include"Renderer.h"
#include"ModelLoader.h"
#include"SceneManager.h"
#include"Arm.h"
#include<thread>

TestObj::TestObj() {

}
void TestObj::Initialize() {

	for (int i = 0; i < 5; i++) {
		m_scale   =(XMFLOAT3(1, 1, 1));
		m_rotation=(XMFLOAT3(0, 0, 0));
		m_position=(XMFLOAT3(0, 0, 0));

	}

	m_arm[0] = new Arm();
	m_arm[1] = new Arm();
	m_arm[0]->Initialize(Arm::Index::LEFT, this);
	m_arm[1]->Initialize(Arm::Index::RIGHT, this);

	//モデルのロードリクエスト
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HEAD);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_EYE);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_ARM);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_LOWER);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_SHOULDER);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_UPPER);

	//定数バッファの作成
	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

}
void TestObj::Update() {
	m_rotation.y += 0.02f;
	m_arm[0]->Update();
	m_arm[1]->Update();
}
void TestObj::Draw() {
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
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HEAD);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_ARM);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_LOWER);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_SHOULDER);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_UPPER);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_EYE);

	for (int i = 0; i < Arm::Index::MAX; i++)
		m_arm[i]->Draw();
}
void TestObj::Finalize() {
	m_arm[0]->Finalize();
	m_arm[1]->Finalize();

	delete m_arm[1];
	delete m_arm[0];
}
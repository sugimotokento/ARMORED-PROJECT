#include"Model.h"

#include"TestObj.h"

#include"TextureGeometry.h"
#include"Scene.h"
#include"Renderer.h"

TestObj::TestObj() {

}
void TestObj::Initialize() {

	for (int i = 0; i < 5; i++) {
		model[i] = new Model();
		m_scale   =(XMFLOAT3(0.005f, 0.005f, 0.005f));
		m_rotation=(XMFLOAT3(0, -1.57f, 0));
		m_position=(XMFLOAT3(2, -5, 0));

		texture[i] = new TextureGeometry();
	}
	model[0]->LoadMesh("asset/model/Dreadnought/Arm.fbx");
	model[1]->LoadMesh("asset/model/Dreadnought/Head.fbx");
	model[2]->LoadMesh("asset/model/Dreadnought/Lower.fbx");
	model[3]->LoadMesh("asset/model/Dreadnought/Shoulder.fbx");
	model[4]->LoadMesh("asset/model/Dreadnought/Upper.fbx");

	std::wstring basePath = L"asset/Texture/Dreadnought/";
	texture[0]->LoadTexture(basePath + L"T_DN_Arm_Albedo.png");
	texture[1]->LoadTexture(basePath + L"T_DN_Head_Albedo.png");
	texture[2]->LoadTexture(basePath + L"T_DN_Lower_Albedo.png");
	texture[3]->LoadTexture(basePath + L"T_DN_Shoulder_Albedo.png");
	texture[4]->LoadTexture(basePath + L"T_DN_Upper_Albedo.png");

	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

}
void TestObj::Update() {

}
void TestObj::Draw() {
	////マトリクス設定
	XMMATRIX lightView = Camera::GetInstance()->GetViewMatrix(Camera::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = Camera::GetInstance()->GetProjectionMatrix(Camera::Index::CAMERA_SHADOW);

	XMMATRIX view = Camera::GetInstance()->GetViewMatrix(Camera::Index::CAMERA_MAIN);
	XMMATRIX projection = Camera::GetInstance()->GetProjectionMatrix(Camera::Index::CAMERA_MAIN);

	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX world = size * rot * trans;

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

	constant->isWater = false;

	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());


	for (int i = 0; i < 5; i++) {
		//テクスチャ設定
		ID3D12DescriptorHeap* dh[] = { texture[i]->GetDescriptorHeap().Get() };
		Renderer::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(1, dh);
		Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(1,
			dh[0]->GetGPUDescriptorHandleForHeapStart()
		);
		model[i]->Draw();
	}
}
void TestObj::Finalize() {

	for (int i = 0; i < 5; i++) {
		delete model[i];

		texture[i]->Finalize();
		delete texture[i];
	}


}
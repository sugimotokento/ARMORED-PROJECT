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
		m_scale   =(XMFLOAT3(0.0025f, 0.0025f, 0.0025f));
		m_rotation=(XMFLOAT3(0, -1.57f, 0));
		m_position=(XMFLOAT3(0, 0, 0));

		texture[i] = new TextureGeometry();
	}
	model[0]->LoadMesh("asset/model/Dreadnought/Arm.fbx");
	model[1]->LoadMesh("asset/model/Dreadnought/Head.fbx");
	model[2]->LoadMesh("asset/model/Dreadnought/Lower.fbx");
	model[3]->LoadMesh("asset/model/Dreadnought/Shoulder.fbx");
	model[4]->LoadMesh("asset/model/Dreadnought/Upper.fbx");

	std::wstring basePath = L"asset/Texture/Dreadnought/";
	texture[0]->LoadTexture(basePath + L"T_DN_Arm_Albedo.tga", basePath + L"T_DN_Arm_NormalMap.png", basePath+L"T_DN_Arm_Occlusion.png", basePath + L"T_DN_Arm_Metallic.png");
	texture[1]->LoadTexture(basePath + L"T_DN_Head_Albedo.tga", basePath + L"T_DN_Head_NormalMap.png", basePath + L"T_DN_Head_Occlusion.png", basePath + L"T_DN_Head_Metallic.png");
	texture[2]->LoadTexture(basePath + L"T_DN_Lower_Albedo.tga", basePath + L"T_DN_Lower_NormalMap.png", basePath + L"T_DN_Lower_Occlusion.png", basePath + L"T_DN_Lower_Metallic.png");
	texture[3]->LoadTexture(basePath + L"T_DN_Shoulder_Albedo.tga", basePath + L"T_DN_Shoulder_NormalMap.png", basePath + L"T_DN_Shoulder_Occlusion.png", basePath + L"T_DN_Shoulder_Metallic.png");
	texture[4]->LoadTexture(basePath + L"T_DN_Upper_Albedo.tga", basePath + L"T_DN_Upper_NormalMap.png", basePath + L"T_DN_Upper_Occlusion.png", basePath + L"T_DN_Upper_Metallic.png");

	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

}
void TestObj::Update() {
	m_rotation.y += 0.02f;
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
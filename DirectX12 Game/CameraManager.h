#pragma once
#include"Main.h"
class  CameraManager
{
public:
	struct Index {
		enum CamraIndex {
			CAMERA_PLAYER,
			CAMERA_SHADOW,
			CAMERA_MAX
		};
	};
private:
	static CameraManager* instance;
	XMMATRIX m_projectionMatrix	[Index::CAMERA_MAX];    
	XMMATRIX m_viewMatrix		[Index::CAMERA_MAX];       
	XMFLOAT3 m_position			[Index::CAMERA_MAX];
	XMFLOAT3 m_target			[Index::CAMERA_MAX];

	//カメラの切り替えタイミングを合わせつために、いったんoldに変更するカメラを入れてUpdateの最初にmainを更新する
	Index::CamraIndex m_mainCamera = Index::CAMERA_PLAYER;
	Index::CamraIndex m_oldMainCamera = Index::CAMERA_PLAYER;

	void UpdatePlayerCamera();
	void UpdateShadowCamera();
public:
	CameraManager();

	void Update();
	void Draw();

	void SetMainCamera(Index::CamraIndex index) { m_oldMainCamera = index; }

	//現在使っているカメラを取得
	XMMATRIX GetMainViewMatrix() { return m_viewMatrix[m_mainCamera]; }
	XMMATRIX GetMainProjectionMatrix() { return m_projectionMatrix[m_mainCamera]; }
	XMFLOAT3 GetMainPosition() { return m_position[m_mainCamera]; }

	XMMATRIX GetViewMatrix(Index::CamraIndex index)			{ return m_viewMatrix[index]; }
	XMMATRIX GetProjectionMatrix(Index::CamraIndex index)	{ return m_projectionMatrix[index]; }
	XMFLOAT3 GetPosition(Index::CamraIndex index)			{ return m_position[index]; }

	static void Create();
	static void Destroy();
	static CameraManager* GetInstance() { return instance; }
};


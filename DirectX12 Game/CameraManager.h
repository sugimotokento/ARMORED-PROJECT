#pragma once
#include"Main.h"
#include<vector>
#include<memory>
#include"Camera.h"
#include"PlayerCamera.h"
#include"ShadowCamera.h"
#include"DebugCamera.h"

class  CameraManager
{
public:
	struct Index {
#define CAMERA_ID(name, className)
#define CAMERA_ID_TABLE\
		CAMERA_ID(PLAYER,	new PlayerCamera)\
		CAMERA_ID(SHADOW,	new ShadowCamera)\

		//�f�o�b�O�p�J����
#define DEBUG_CAMERA_ID(name, calssName)
#define DEBUG_CAMERA_ID_TABLE\
		DEBUG_CAMERA_ID(DEBUG, new DebugCamera)\


		enum CameraIndex {
#undef CAMERA_ID
#define CAMERA_ID(name, className) CAMERA_##name,
			CAMERA_ID_TABLE
#undef DEBUG_CAMERA_ID

		
#ifdef _DEBUG	//�f�o�b�O�p�J����
#define DEBUG_CAMERA_ID(name, className) CAMERA_##name,
			DEBUG_CAMERA_ID_TABLE
#endif
			CAMERA_MAX
		};


		static Camera* CreateCamera(CameraIndex id) {
#undef CAMERA_ID
#define CAMERA_ID(name, className)\
		case CAMERA_##name:\
				return  className();


#ifdef _DEBUG//�f�o�b�O�p�J����
#undef DEBUG_CAMERA_ID
#define DEBUG_CAMERA_ID(name, className)\
		case CAMERA_##name:\
				return  className();
#endif
			switch (id) {
				CAMERA_ID_TABLE
			}

#ifdef _DEBUG
			//�f�o�b�O�p�J����
			switch (id) {
				DEBUG_CAMERA_ID_TABLE
			}
#endif
			return nullptr;
		}
	};
private:
	static CameraManager* instance;
	Camera* m_camera[Index::CAMERA_MAX];


	//�J�����̐؂�ւ��^�C�~���O�����킹���߂ɁA��������old�ɕύX����J����������Update�̍ŏ���main���X�V����
	Index::CameraIndex m_mainCamera = Index::CAMERA_PLAYER;
	Index::CameraIndex m_oldMainCamera = Index::CAMERA_PLAYER;

public:
	CameraManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	Index::CameraIndex GetMainCameraIndex() { return m_mainCamera; }
	void SetMainCamera(Index::CameraIndex index) { m_oldMainCamera = index; }
	void SetMainCameraPosition(XMFLOAT3 pos) { m_camera[m_mainCamera]->SetPosition(pos); }

	//���ݎg���Ă���J�������擾
	XMMATRIX GetMainViewMatrix() { return m_camera[m_mainCamera]->GetViewMatrix(); }
	XMMATRIX GetMainProjectionMatrix() { return m_camera[m_mainCamera]->GetProjectionMatrix(); }
	XMFLOAT3 GetMainPosition() { return m_camera[m_mainCamera]->GetPosition(); }
	XMFLOAT3 GetMainTarget() { return m_camera[m_mainCamera]->GetTarget(); }

	XMMATRIX GetViewMatrix(Index::CameraIndex index)			{ return m_camera[index]->GetViewMatrix();}
	XMMATRIX GetProjectionMatrix(Index::CameraIndex index)	{ return m_camera[index]->GetProjectionMatrix(); }
	XMFLOAT3 GetPosition(Index::CameraIndex index)			{ return m_camera[index]->GetPosition(); }
	XMFLOAT3 GetTarget(Index::CameraIndex index)				{ return m_camera[index]->GetTarget(); }

	static void Create();
	static void Destroy();
	static CameraManager* GetInstance() { return instance; }
};


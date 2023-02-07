#pragma once
#include"Main.h"
class  Camera
{
public:
	struct Index {
		enum CamraIndex {
			CAMERA_MAIN,
			CAMERA_SHADOW,
			CAMERA_MAX
		};
	};
private:
	static Camera* instance;
	XMMATRIX m_projectionMatrix	[Index::CAMERA_MAX];    
	XMMATRIX m_viewMatrix		[Index::CAMERA_MAX];       
	XMFLOAT3 m_position			[Index::CAMERA_MAX];
	XMFLOAT3 m_target			[Index::CAMERA_MAX];

	void UpdateMainCamera();
	void UpdateShadowCamera();
public:
	Camera();

	void Update();
	void Draw();

	XMMATRIX GetViewMatrix(Index::CamraIndex index)			{ return m_viewMatrix[index]; }
	XMMATRIX GetProjectionMatrix(Index::CamraIndex index)	{ return m_projectionMatrix[index]; }
	XMFLOAT3 GetPosition(Index::CamraIndex index)			{ return m_position[index]; }

	static void Create();
	static void Destroy();
	static Camera* GetInstance() { return instance; }
};


#include"DebugCamera.h"
#include"Input.h"
#include"XMMath.h"
#include"ImguiRenderer.h"
#include<math.h>

DebugCamera::DebugCamera() {
	m_position = XMFLOAT3(5, 2, -5);
	m_target = XMFLOAT3(0.0f, 0, 5.0f);
	m_rotation = XMFLOAT3(0, 0, 0);

	XMVECTOR quat = XMQuaternionIdentity();
	XMStoreFloat4(&m_quaternion, quat);

	quat = XMLoadFloat4(&m_quaternion);
	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationQuaternion(quat);
	XMMATRIX w = rot * trans;
	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, w);

	XMFLOAT3 right = { world._11, world._12, world._13 };
	XMFLOAT3 up = { world._21, world._22, world._23 };
	XMFLOAT3 forward = { world._31, world._32, world._33 };
	right = XMMath::Normalize(right);
	up = XMMath::Normalize(up);
	forward = XMMath::Normalize(forward);


	FXMVECTOR upVec = XMLoadFloat3(&up);
	XMVECTOR q = XMQuaternionRotationAxis(upVec,-0.5f);
	q = XMQuaternionMultiply(XMLoadFloat4(&m_quaternion), q);
	XMStoreFloat4(&m_quaternion, q);

#ifdef _DEBUG
	std::function<bool()> f = std::bind(&DebugCamera::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f);
#endif // _DEBUG
}
void DebugCamera::Update() {
	
	FXMVECTOR quat = XMLoadFloat4(&m_quaternion);
	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationQuaternion(quat);
	XMMATRIX w = rot * trans;
	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, w);

	XMFLOAT3 right = { world._11, world._12, world._13 };
	XMFLOAT3 up = { world._21, world._22, world._23 };
	XMFLOAT3 forward = { world._31, world._32, world._33 };
	right=XMMath::Normalize(right);
	up =XMMath::Normalize(up);
	forward =XMMath::Normalize(forward);


	

	if (fabsf(XInput::GetInstance()->GetRightThumb().x) > 0.01f) {
		m_quaternion=XMMath::QuaternionRotateAxis(m_quaternion, up, XInput::GetInstance()->GetRightThumb().x * 0.04f);
	}
	if (fabsf(XInput::GetInstance()->GetRightThumb().y) > 0.01f) {
		m_quaternion=XMMath::QuaternionRotateAxis(m_quaternion, right, -XInput::GetInstance()->GetRightThumb().y * 0.04f);
	}


	if (fabsf(XInput::GetInstance()->GetLeftThumb().x) > 0.01f) {
		m_position += right * 0.04f* XInput::GetInstance()->GetLeftThumb().x;
	}
	if (fabsf(XInput::GetInstance()->GetLeftThumb().y) > 0.01f) {
		m_position += forward * 0.04f* XInput::GetInstance()->GetLeftThumb().y;
	}

	m_target = m_position+  forward*5;
}
void DebugCamera::Draw() {
	Camera::Draw();
}


#ifdef _DEBUG
bool DebugCamera::ImguiDebug() {


	//XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	//XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	//XMMATRIX w = rot * trans;
	//XMFLOAT4X4 world;
	//XMStoreFloat4x4(&world, w);

	//XMFLOAT3 right = { world._11, world._12, world._13 };
	//XMFLOAT3 up = { world._21, world._22, world._23 };
	//XMFLOAT3 forward = { world._31, world._32, world._33 };

	//ImGui::Begin("DebugCamera");
	//ImGui::Text("Input");
	//ImGui::Text("  x:%lf, y:%lf", XInput::GetInstance()->GetRightThumb().x, XInput::GetInstance()->GetRightThumb().y);
	//ImGui::Text("");
	//ImGui::Text("forward");
	//ImGui::Text("x:%lf, y:%lf, z:%lf", forward.x, forward.y, forward.z);
	//ImGui::Text("");
	//ImGui::Text("Quaternion");
	//ImGui::Text("x:%lf, y:%lf, z:%lf, w:%lf", m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	//ImGui::End();
	return true;
}
#endif // _DEBUG
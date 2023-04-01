#include"XMMath.h"


XMFLOAT3 XMMath::Normalize(const XMFLOAT3& vec) {
	XMVECTOR vector=XMLoadFloat3(&vec);
	vector = XMVector3Normalize(vector);

	XMFLOAT3 out;
	XMStoreFloat3(&out, vector);

	return out;
}
float XMMath::Dot(const XMFLOAT3& vec1, const XMFLOAT3& vec2) {
	XMVECTOR v1 = XMLoadFloat3(&vec1);
	XMVECTOR v2 = XMLoadFloat3(&vec2);

	XMVECTOR nv = XMVector3Dot(v1, v2);
	float out;
	XMStoreFloat(&out, nv);

	return out;

}
XMFLOAT3 XMMath::Cross(const XMFLOAT3& vec1, const XMFLOAT3& vec2) {
	XMVECTOR v1 = XMLoadFloat3(&vec1);
	XMVECTOR v2 = XMLoadFloat3(&vec2);

	XMVECTOR nv = XMVector3Cross(v1, v2);
	XMFLOAT3 out;
	XMStoreFloat3(&out, nv);

	return out;
}

float XMMath::Length(const XMFLOAT3& vec) {
	XMVECTOR vector = XMLoadFloat3(&vec);

	vector = XMVector3Length(vector);
	float len;
	XMStoreFloat(&len, vector);

	return len;
}

XMFLOAT4 XMMath::QuaternionRotateAxis(const XMFLOAT4& quat, const XMFLOAT3& axis, float angle) {
	XMVECTOR axisVec = XMLoadFloat3(&axis);
	XMVECTOR quatTemp = XMQuaternionRotationAxis(axisVec, angle);
	quatTemp = XMQuaternionMultiply(XMLoadFloat4(&quat), quatTemp);

	XMFLOAT4 out;
	XMStoreFloat4(&out, quatTemp);
	return out;
}
XMFLOAT4 XMMath::QuaternionSetRotation(const XMFLOAT3& axis, float angle) {
	XMVECTOR axisVec = XMLoadFloat3(&axis);
	XMVECTOR quatTemp = XMQuaternionRotationAxis(axisVec, angle);

	XMFLOAT4 out;
	XMStoreFloat4(&out, quatTemp);
	return out;
}
XMFLOAT4 XMMath::QuaternionNormalize(const XMFLOAT4& quat) {
	XMVECTOR quatTemp = XMLoadFloat4(&quat);
	quatTemp = XMQuaternionNormalize(quatTemp);

	XMFLOAT4 out;
	XMStoreFloat4(&out, quatTemp);
	return out;
}
#pragma once
#include"Main.h"
#include"GameObject.h"

class Ocean :public GameObject {
private:
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_constantBuffer;
	XMFLOAT4 m_waterParam;
 


public:
	Ocean();
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};
#pragma once
#include "GameObject.h"

class Model;
class TextureGeometry;
class TestObj :public GameObject {
private:
	Model* model[5];
	TextureGeometry* texture[5];
	ComPtr<ID3D12Resource> m_constantBuffer;

public:
	TestObj();
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};
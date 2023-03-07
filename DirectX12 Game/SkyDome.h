#pragma once
#include"GameObject.h"
#include"Model.h"
#include"TextureGeometry.h"
#include <memory>

class Model;
class SkyDome : public GameObject {
private:
	std::unique_ptr<Model> model;
	ComPtr<ID3D12Resource> m_constantBuffer;
	std::unique_ptr<TextureGeometry> texture;
public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};
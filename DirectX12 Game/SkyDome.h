#pragma once
#include"GameObject.h"
#include"Model.h"
#include <memory>

class Model;
class SkyDome : public GameObject {
private:
	std::unique_ptr<Model> model;
	ComPtr<ID3D12Resource> m_constantBuffer;
public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};
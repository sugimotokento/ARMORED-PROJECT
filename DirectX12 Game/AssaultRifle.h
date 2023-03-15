#pragma once
#include"Weapon.h"
#include"TextureGeometry.h"
#include"Model.h"
#include<memory>

class AssaultRifle : public Weapon {
private:
	std::unique_ptr<Model> m_model;
	std::unique_ptr<TextureGeometry> m_texture;
public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};
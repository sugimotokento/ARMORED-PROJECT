#pragma once
#include"Main.h"
#include"GameObject.h"

class Weapon :GameObject{
protected:
	XMFLOAT3 m_offset;//手や肩に位置を合わせるoffset
	GameObject* m_parent;
	bool m_isReload = false;
public:
	virtual void Shot() = 0;
	virtual void Initialize(){}
	virtual void Update(){}
	virtual void Draw(){}
	virtual void Finalize(){}

	bool GetIsReload() { return m_isReload; }
};
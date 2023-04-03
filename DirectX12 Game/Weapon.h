#pragma once
#include"Main.h"
#include"GameObject.h"

class Weapon :public GameObject{
protected:
	float m_intervalMax = 20;
	float m_intervalCount = 0;
public:

	virtual void Shot() = 0;
	virtual void Initialize(){}
	virtual void Update(){}
	virtual void Draw(){}
	virtual void Finalize(){}

	float GetIntervalMax() { return m_intervalMax; }
	float GetIntervalCount() { return m_intervalCount; }

};
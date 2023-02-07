#pragma once
#include"Gun.h"

class NormalGun :public Gun {
private:
	const int SHOT_INTERVAL = 8;
	int m_shotIntervalCount;

public:
	NormalGun();
	~NormalGun(){}
	void Update() final override;
};
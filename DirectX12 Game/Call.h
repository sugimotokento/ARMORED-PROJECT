#pragma once
#include<functional>
#include<vector>

class Call {
private:
	static Call *instance;
	std::vector<std::function<void()>> m_delayFunction;
	std::vector<int> m_count;
	std::vector<int> m_countMax;

public:
	Call() { instance = this; }
	void DelayUpdate();
	void AddDelayFunction(const std::function<void()>& func, int countMax);

	static Call* GetInstance() { return instance; }
};
#include"Call.h"
Call* Call::instance=nullptr;

void Call::DelayUpdate() {
	for (int i = 0; i < m_delayFunction.size(); i++) {
		if (m_count[i] >= m_countMax[i]) {
			m_delayFunction[i]();
			m_delayFunction.erase(m_delayFunction.begin() + i);
			m_count.erase(m_count.begin() + i);
			m_countMax.erase(m_countMax.begin() + i);

			if (m_delayFunction.size() <= 0)break;
			else continue;
		}
		m_count[i]++;
	}
}

void Call::AddDelayFunction(const std::function<void()>& func, int countMax) {
	m_delayFunction.push_back(func);
	m_count.push_back(0);
	m_countMax.push_back(countMax);
}
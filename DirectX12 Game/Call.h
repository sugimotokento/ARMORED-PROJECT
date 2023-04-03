#pragma once
#include<functional>
#include<vector>


//-------------Callに関数を登録する方法------------------------//
// クラスに非同期実行を記述する関数を書く{void 関数名()}
//	std::function<void()> f = std::bind(&クラス名::関数名, this);
//	Call::GetInstance()->AddDelayFunction(f, 遅延させたいフレーム数);
//これでCallに関数を登録して非同期実行できる
//--------------------------------------------------------------------//
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
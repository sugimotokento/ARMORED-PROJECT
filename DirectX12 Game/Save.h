#pragma once
#include<vector>
#include<string>

struct DataFloat {
	float value;
	char name[21];
};
struct DataInt {
	int value;
	char name[21];
};
struct DataBool {
	bool value;
	char name[21];
};

class Save {
private:
	static Save* m_instance;

	//vectorのサイズを保存しとく用
	unsigned int dataSizeFloat;
	unsigned int dataSizeBool;
	unsigned int dataSizeInt;


	//セーブするデータをここに入れる
	std::vector<DataFloat> dataFloat;
	std::vector<DataInt> dataInt;
	std::vector<DataBool> dataBool;


public:
	void Write(); //最後に1回だけ呼ぶ
	void Load();  //最初に1回だけ呼ぶ



	//名前は20文字まで
	//呼び出すための名前とセーブする値を入れる
	void SetFloat(std::string name, float value);
	void SetInt(std::string name, int value);
	void SetBool(std::string name, bool value);

	//セット関数のnameをキーにして呼び出す
	float GetFloat(std::string name);
	int GetInt(std::string name);
	bool GetBool(std::string name);


	//データの全削除
	void DeleteAllFloat();
	void DeleteAllInt();
	void DeleteAllBool();

	//キーで削除
	void DeleteKeyFloat(std::string name);
	void DeleteKeyInt(std::string name);
	void DeleteKeyBool(std::string name);


	static void Create();
	static void Destroy();
	static Save* GetInstance();
};
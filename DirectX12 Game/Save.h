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
	//vectorのサイズを保存しとく用
	static unsigned int dataSizeFloat;
	static unsigned int dataSizeBool;
	static unsigned int dataSizeInt;


	//セーブするデータをここに入れる
	static std::vector<DataFloat> dataFloat;
	static std::vector<DataInt> dataInt;
	static std::vector<DataBool> dataBool;


public:
	static void Write(); //最後に1回だけ呼ぶ
	static void Load();  //最初に1回だけ呼ぶ



	//名前は20文字まで
	//呼び出すための名前とセーブする値を入れる
	static void SetFloat(std::string name, float value);
	static void SetInt(std::string name, int value);
	static void SetBool(std::string name, bool value);

	//セット関数のnameをキーにして呼び出す
	static float GetFloat(std::string name);
	static int GetInt(std::string name);
	static bool GetBool(std::string name);


	//データの全削除
	static void DeleteAllFloat();
	static void DeleteAllInt();
	static void DeleteAllBool();

	//キーで削除
	static void DeleteKeyFloat(std::string name);
	static void DeleteKeyInt(std::string name);
	static void DeleteKeyBool(std::string name);
};
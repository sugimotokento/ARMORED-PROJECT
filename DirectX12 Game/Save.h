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
	//vector�̃T�C�Y��ۑ����Ƃ��p
	static unsigned int dataSizeFloat;
	static unsigned int dataSizeBool;
	static unsigned int dataSizeInt;


	//�Z�[�u����f�[�^�������ɓ����
	static std::vector<DataFloat> dataFloat;
	static std::vector<DataInt> dataInt;
	static std::vector<DataBool> dataBool;


public:
	static void Write(); //�Ō��1�񂾂��Ă�
	static void Load();  //�ŏ���1�񂾂��Ă�



	//���O��20�����܂�
	//�Ăяo�����߂̖��O�ƃZ�[�u����l������
	static void SetFloat(std::string name, float value);
	static void SetInt(std::string name, int value);
	static void SetBool(std::string name, bool value);

	//�Z�b�g�֐���name���L�[�ɂ��ČĂяo��
	static float GetFloat(std::string name);
	static int GetInt(std::string name);
	static bool GetBool(std::string name);


	//�f�[�^�̑S�폜
	static void DeleteAllFloat();
	static void DeleteAllInt();
	static void DeleteAllBool();

	//�L�[�ō폜
	static void DeleteKeyFloat(std::string name);
	static void DeleteKeyInt(std::string name);
	static void DeleteKeyBool(std::string name);
};
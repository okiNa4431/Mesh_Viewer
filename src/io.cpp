#include "io.h"
#include <iostream>
#include <fstream>

string GetExtension(const string& filePath)
{
	int idx = filePath.rfind('.');
	return filePath.substr(idx + 1, filePath.size() - idx - 1);
}

io& io::Instance()
{
	static io Instance;
	return Instance;
}

void io::LoadMesh(const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices)
{
	string ext = GetExtension(filePath);
	auto it = _ext2readFunc.find(ext);
	//�g���q����K�؂ȃ��b�V���ǂݍ��ݗp�֐����Ăяo��
	if (it != _ext2readFunc.end())
	{
		it->second(filePath, vertices, indices);
	}
	else
	{
		printf("Not Set Extension.\n");
	}
}

bool io::Read_ply(const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices)
{
	//ASCII�`����PLY�t�@�C���ǂ݂���
	printf("Reading PLY File.\n");
	ifstream ifs(filePath);
	if (!ifs)
	{
		printf("Not Found File\n");
		return false;
	}
	string headerStr;
	unsigned int vertNum = 0;
	unsigned int indiceNum = 0;
	while (headerStr != "end_header" && !ifs.eof())
	{
		ifs >> headerStr;
		if (headerStr == "comment") getline(ifs, headerStr);
		else if (headerStr == "elenemt")
		{
			string elementInfo;
			ifs >> elementInfo;
			if (elementInfo == "vertex")
			{
				ifs >> vertNum;
			}
			else if (elementInfo == "face")
			{
				ifs >> indiceNum;
			}
		}
		//�{����property�ɉ����ă��C�A�E�g�Ƃ������肵����
		else if (headerStr == "property")
		{

		}
	}
	printf("vertex: %u\n", vertNum);
	printf("face: %u\n", indiceNum);
	return true;
}

io::io()
{
	//�����ɑ��̃f�[�^�`���������A�˂Ă���
	_ext2readFunc["ply"] = [](const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices)->bool
		{ return &io::Read_ply; };
}
io::~io()
{
}
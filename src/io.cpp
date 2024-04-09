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
	ifstream ifs(filePath, ios::binary);
	if (!ifs)
	{
		printf("Not Found File\n");
		return false;
	}
	string headerStr;
	unsigned int vertNum = 0;
	unsigned int indiceNum = 0;
	bool IsASCII = true;
	while (headerStr != "end_header" && !ifs.eof())
	{
		ifs >> headerStr;
		if (headerStr == "comment") getline(ifs, headerStr);
		else if (headerStr == "element")
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
		if (headerStr == "ascii") IsASCII = true;
		else IsASCII = false;
	}
	printf("vertex: %u\n", vertNum);
	printf("face: %u\n", indiceNum);

	//���_�f�[�^�ǂݍ���
	if (IsASCII)
	{
		for (int i = 0; i < vertNum; i++)
		{
			float x, y, z;
			ifs >> x >> y >> z;
			const unsigned char* xPtr = reinterpret_cast<const unsigned char*>(&x);
			const unsigned char* yPtr = reinterpret_cast<const unsigned char*>(&y);
			const unsigned char* zPtr = reinterpret_cast<const unsigned char*>(&z);
			vertices.insert(vertices.end(), xPtr, xPtr + sizeof(float));
			vertices.insert(vertices.end(), yPtr, yPtr + sizeof(float));
			vertices.insert(vertices.end(), zPtr, zPtr + sizeof(float));
		}

		for (int i = 0; i < indiceNum; i++)
		{
			unsigned int topo,x,y,z;
			ifs >> topo >> x >> y >> z;
			indices.push_back(x);
			indices.push_back(y);
			indices.push_back(z);
		}
	}
	else
	{

	}

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
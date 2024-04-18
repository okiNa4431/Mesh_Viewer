#include "io.h"
#include <cassert>
#include <array>
#include <DirectXMath.h>

using namespace DirectX;

string GetExtension(const string& filePath)
{
	int idx = filePath.rfind('.');
	return filePath.substr(idx + 1, filePath.size() - idx - 1);
}

//以下、メッシュの読み込み関数
bool Read_ply(const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices)
{
	//ASCII形式のPLYファイル読みこみ
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
	bool haveNormal = false;
	while (headerStr != "end_header" && !ifs.eof())
	{
		ifs >> headerStr;
		if (headerStr == "comment") getline(ifs, headerStr);
		else if (headerStr == "format")
		{
			string format;
			ifs >> format;
			if (format.find("ascii") != string::npos) IsASCII = true;
			else IsASCII = false;
		}
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
		//本当はpropertyに応じてレイアウトとかを決定したい
		else if (headerStr == "property")
		{
			string format;
			ifs >> format;
			string property;
			ifs >> property;
			if (property == "nx") haveNormal = true;
		}
	}
	printf("vertex: %u, face: %u\n", vertNum, indiceNum);

	//頂点データ読み込み
	vector<XMVECTOR> position(vertNum);
	if (IsASCII)
	{
		for (int i = 0; i < vertNum; i++)
		{
			float x, y, z;
			ifs >> x >> y >> z;
			position[i] = XMVectorSet(x, y, z, 0.0f);
		}

		for (int i = 0; i < indiceNum; i++)
		{
			unsigned int topo, x, y, z;
			ifs >> topo >> x >> y >> z;
			indices.push_back(x);
			indices.push_back(y);
			indices.push_back(z);
		}
	}
	else
	{
		//ifs.read(reinterpret_cast<char*>(vertices.data()), vertNum * 12);
		//ifs.read(reinterpret_cast<char*>(indices.data()), indiceNum * sizeof(indices[0]));
	}

	//法線を持っていない場合はここで法線を計算
	vector<vector<XMVECTOR>> vertNormals(vertNum,vector<XMVECTOR>(0));
	if (!haveNormal)
	{
		for (int i = 0; i < indiceNum; i++)
		{
			const unsigned int a = indices[i * 3];
			const unsigned int b = indices[i * 3+1];
			const unsigned int c = indices[i * 3+2];
			const XMVECTOR abVec = XMVectorSubtract(position[b], position[a]);
			const XMVECTOR acVec = XMVectorSubtract(position[c], position[a]);
			const XMVECTOR normal = XMVector3Normalize(XMVector3Cross(abVec, acVec));
			vertNormals[a].push_back(normal);
			vertNormals[b].push_back(normal);
			vertNormals[c].push_back(normal);
		}
	}

	//ここでverticeにデータを格納
	for (int i = 0; i < vertNum; i++)
	{
		//座標情報
		for (int j = 0; j < 3; j++)
		{
			const float posJ = XMVectorGetByIndex(position[i], j);
			const unsigned char* posJPtr = reinterpret_cast<const unsigned char*>(&posJ);
			vertices.insert(vertices.end(), posJPtr, posJPtr + sizeof(float));
		}

		//法線情報
		if (!haveNormal)
		{
			XMVECTOR normal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			for (int j = 0; j < (int)vertNormals[i].size(); j++)
			{
				normal = XMVectorAdd(normal, vertNormals[i][j]);
			}
			normal = XMVector3Normalize(normal);

			for (int j = 0; j < 3; j++)
			{
				const float norJ = XMVectorGetByIndex(normal, j);
				const unsigned char* norJPtr = reinterpret_cast<const unsigned char*>(&norJ);
				vertices.insert(vertices.end(), norJPtr, norJPtr + sizeof(float));
			}
		}
		
	}

	ifs.close();
	return true;
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
	//拡張子から適切なメッシュ読み込み用関数を呼び出し
	if (it != _ext2readFunc.end())
	{
		printf("%s: Found Extension.\n", ext.c_str());
		it->second(filePath, vertices, indices);
	}
	else
	{
		printf("%s: Not Set Extension.\n",filePath.c_str());
	}
}

io::io()
{
	//ここに他のデータ形式を書き連ねていく
	_ext2readFunc["ply"] = [](const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices)->bool
		{ return Read_ply(filePath, vertices, indices); };
}
io::~io()
{
}
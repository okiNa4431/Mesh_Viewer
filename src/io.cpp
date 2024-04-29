#include "io.h"
#include <cassert>
#include <array>
#include <DirectXMath.h>
#include <sstream>

using namespace DirectX;

string GetExtension(const string& filePath)
{
	int idx = filePath.rfind('.');
	return filePath.substr(idx + 1, filePath.size() - idx - 1);
}

//以下、メッシュの読み込み関数
bool Read_ply(const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices)
{
	printf("Reading PLY File.\n");
	ifstream ifs(filePath.c_str(), ios::in | ios::binary);
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
		else if (headerStr == "end_header")
		{
			getline(ifs, headerStr);
			break;
		}
	}
	printf("vertex: %u, face: %u\n", vertNum, indiceNum);

	//頂点データ読み込み
	vector<XMFLOAT3> position(vertNum);
	if (IsASCII)
	{
		for (int i = 0; i < vertNum; i++)
		{
			float x, y, z;
			ifs >> x >> y >> z;
			position[i] = XMFLOAT3(x, y, z);
		}

		for (int i = 0; i < indiceNum; i++)
		{
			uint8_t topo;
			ifs >> topo;
			if (topo == 3)
			{
				unsigned int x, y, z;
				ifs >> x >> y >> z;
				indices.push_back(x);
				indices.push_back(y);
				indices.push_back(z);
			}
			else
			{
				unsigned int x, y, z, w;
				ifs >> x >> y >> z >> w;
				indices.push_back(x);
				indices.push_back(y);
				indices.push_back(z);
				indices.push_back(x);
				indices.push_back(z);
				indices.push_back(w);
			}
		}
	}
	else
	{
		for (int i = 0; i < vertNum; i++)
		{
			float pos[3];
			ifs.read((char*)pos, sizeof(float) * 3);
			position[i] = XMFLOAT3(pos[0], pos[1], pos[2]);
		}
		//ifs.read(reinterpret_cast<char*>(position.data()), vertNum * sizeof(XMFLOAT3));
		for (int i = 0; i < indiceNum; i++)
		{
			//プロパティ数を得る
			uint8_t buff;
			ifs.read(reinterpret_cast<char*>(&buff), sizeof(uint8_t));

			//プロパティ(主にインデックス情報)を読み込んで格納
			if (buff == 3)
			{
				uint32_t index_line[3];
				ifs.read(reinterpret_cast<char*>(&index_line), sizeof(indices[0]) * 3);
				indices.push_back(index_line[0]);
				indices.push_back(index_line[1]);
				indices.push_back(index_line[2]);
			}
			else if (buff == 4)
			{
				uint32_t index_line[4];
				ifs.read(reinterpret_cast<char*>(&index_line), sizeof(indices[0]) * 4);
				indices.push_back(index_line[0]);
				indices.push_back(index_line[1]);
				indices.push_back(index_line[2]);
				indices.push_back(index_line[0]);
				indices.push_back(index_line[2]);
				indices.push_back(index_line[3]);
			}
			
		}
		//ifs.read(reinterpret_cast<char*>(indices.data()), indiceNum * 4 * sizeof(indices[0]));
	}
	/*for (int i = 0; i < indiceNum*4; i++)
	{
		cout << i << " "<<indices[i]<<endl;
	}*/

	//法線を持っていない場合はここで法線を計算
	vector<vector<XMVECTOR>> vertNormals(vertNum, vector<XMVECTOR>(0));
	if (!haveNormal)
	{
		for (int i = 0; i < indiceNum; i++)
		{
			const unsigned int a = indices[i * 3];
			const unsigned int b = indices[i * 3+1];
			const unsigned int c = indices[i * 3+2];
			const XMVECTOR abVec = XMVectorSubtract(XMLoadFloat3(&position[b]), XMLoadFloat3(&position[a]));
			const XMVECTOR acVec = XMVectorSubtract(XMLoadFloat3(&position[c]), XMLoadFloat3(&position[a]));
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
			const float posJ = XMVectorGetByIndex(XMLoadFloat3(&position[i]), j);
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

bool Read_obj(const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices)
{
	//ファイルを開く
	printf("Reading OBJ File.\n");
	ifstream ifs(filePath.c_str(), ios::in);
	if (!ifs)
	{
		printf("Not Found File\n");
		return false;
	}

	//読み込み
		//面情報を読み込むときに使うフラグ
	bool existPos = false;
	bool existNor = false;
	bool existTex = false;
	string key;	//各行冒頭のキーワードを読み込む
	float vertexData[3]; //一旦書き込む用の配列
	vector<XMFLOAT3> positions; //座標書き込み用
	vector<XMVECTOR> normals; //法線書き込み用
	vector<XMFLOAT2> textureUV; //テクスチャ座標書き込み用
	vector<vector<int>> index_each_param(0, vector<int>(3,0)); //インデックス書き込み用
	const vector<int> initializeInts = { 0,0,0 };
	while (!ifs.eof())
	{
		ifs >> key;
		if (key[0] == '#') getline(ifs, key);
		else if (key == "v")
		{
			existPos = true;
			ifs >> vertexData[0] >> vertexData[1] >> vertexData[2];
			positions.push_back(XMFLOAT3(vertexData[0], vertexData[1], vertexData[2]));
		}
		else if (key == "vn")
		{
			existNor = true;
			ifs >> vertexData[0] >> vertexData[1] >> vertexData[2];
			XMVECTOR n = { vertexData[0],vertexData[1],vertexData[2] };
			normals.push_back(n);
		}
		else if (key == "vt")
		{
			existTex = true;
			ifs >> vertexData[0] >> vertexData[1];
			textureUV.push_back(XMFLOAT2(vertexData[0], vertexData[1]));
		}
		else if (key == "f")
		{
			//面情報を一列分取る
			getline(ifs, key);
			if (!key.empty())
			{
				istringstream line(key);
				string vert_str;
				//各頂点の情報を取る
				while (getline(line, vert_str, ' '))
				{
					if (!vert_str.empty())
					{
						//各頂点の各パラメータの情報を取る
						int i = 0;
						index_each_param.push_back(initializeInts);
						istringstream vertStream(vert_str);
						string para_str;
						while (getline(vertStream, para_str, '/'))
						{
							if (!para_str.empty())
							{
								int idx = stoi(para_str);
								index_each_param[(int)index_each_param.size() - 1][i] = idx;
							}
							i++;
						}
					}
				}
			}
		}
	}

	//引数の配列に格納
	//flat shadingのために面毎に頂点を作る
	const unsigned int vertNum = (unsigned int)index_each_param.size();
	const unsigned int indiceNum = (unsigned int)index_each_param.size();
	printf("vertex: %u, face: %u\n", vertNum, indiceNum);

	//法線情報がない時は計算する
	if (!existNor)
	{
		normals.resize(vertNum);
		for (int i = 0; i < vertNum/3; i++)
		{
			const XMFLOAT3 aPos = positions[index_each_param[3*i][0] - 1];
			const XMFLOAT3 bPos = positions[index_each_param[3*i+1][0] - 1];
			const XMFLOAT3 cPos = positions[index_each_param[3*i+2][0] - 1];
			const XMVECTOR abVec = XMVectorSubtract(XMLoadFloat3(&bPos), XMLoadFloat3(&aPos));
			const XMVECTOR acVec = XMVectorSubtract(XMLoadFloat3(&cPos), XMLoadFloat3(&aPos));
			const XMVECTOR normal = XMVector3Normalize(XMVector3Cross(abVec, acVec));
			normals[3 * i] = normal;
			normals[3 * i + 1] = normal;
			normals[3 * i + 2] = normal;
			index_each_param[3 * i][2] = 3 * i+1;
			index_each_param[3 * i + 1][2] = 3 * i + 2;
			index_each_param[3 * i + 2][2] = 3 * i + 3;
		}
	}

	//ここでverticeにデータを格納
	for (int i = 0; i < vertNum; i++)
	{
		//座標情報
		for (int j = 0; j < 3; j++)
		{
			const float posJ = XMVectorGetByIndex(XMLoadFloat3(&positions[index_each_param[i][0]-1]), j);
			const unsigned char* posJPtr = reinterpret_cast<const unsigned char*>(&posJ);
			vertices.insert(vertices.end(), posJPtr, posJPtr + sizeof(float));
		}

		//法線情報
		for (int j = 0; j < 3; j++)
		{
			const float posJ = XMVectorGetByIndex(XMVector3Normalize(normals[index_each_param[i][2] - 1]), j);
			const unsigned char* posJPtr = reinterpret_cast<const unsigned char*>(&posJ);
			vertices.insert(vertices.end(), posJPtr, posJPtr + sizeof(float));
		}
	}

	//indicesにデータを格納
	for (int i = 0; i < indiceNum; i++) indices.push_back(i);

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
	_ext2readFunc["obj"] = [](const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices)->bool
		{ return Read_obj(filePath, vertices, indices); };
}
io::~io()
{
}
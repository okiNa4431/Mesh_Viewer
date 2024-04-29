#include <filesystem>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <sstream>

using namespace std;

int main() {
	//ファイルを開く
	printf("Reading OBJ File.\n");
	ifstream ifs("C:\\Users\\NaokiMurakami\\3D Objects\\bunny_obj.obj", ios::in);
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
	while (!ifs.eof())
	{
		ifs >> key;
		if (key[0] == '#')
		{
			getline(ifs, key);
		}
		else if (key == "v")
		{
			existPos = true;
			ifs >> vertexData[0] >> vertexData[1] >> vertexData[2];
			//cout << vertexData[0] << " " << vertexData[1] << " " << vertexData[2] << endl;
		}
		else if (key == "vn")
		{
			existNor = true;
			ifs >> vertexData[0] >> vertexData[1] >> vertexData[2];
			//cout << vertexData[0] << " " << vertexData[1] << " " << vertexData[2] << endl;
		}
		else if (key == "vt")
		{
			existTex = true;
			ifs >> vertexData[0] >> vertexData[1];
		}
		else if (key == "f")
		{
			getline(ifs, key);
			if (!key.empty())
			{
				//cout << key << endl;
				istringstream vertex(key);
				string num;
				while (getline(vertex, num, '/'))
				{
					if (!num.empty())
					{
						int idx = stoi(num);
						cout << idx << " ";
					}
				}
				cout << endl;
			}
		}
	}
}
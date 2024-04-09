#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main()
{
	ifstream ifs("C:\\Users\\NaokiMurakami\\3D Objects\\walkman.ply", ios::binary);
	if (!ifs.is_open())
	{
		printf("Not Found File\n");
		return 0;
	}

	string headerStr;
	unsigned int vertNum = 0;
	unsigned int indiceNum = 0;
	while (headerStr != "end_header" && !ifs.eof())
	{
		ifs >> headerStr;
		cout << headerStr << " ";
		if (headerStr == "comment")
		{
			getline(ifs, headerStr);
			cout << headerStr << endl;
		}
		else if (headerStr == "element")
		{
			string elementInfo;
			ifs >> elementInfo;
			cout << elementInfo << " ";
			if (elementInfo == "vertex")
			{
				ifs >> vertNum;
				cout << vertNum << endl;
			}
			else if (elementInfo == "face")
			{
				ifs >> indiceNum;
				cout << indiceNum << endl;
			}
		}
		//本当はpropertyに応じてレイアウトとかを決定したい
		else if (headerStr == "property")
		{

		}
	}
	printf("vertex: %u\n", vertNum);
	printf("face: %u\n", indiceNum);

	ifs.close();
	return 0;
}
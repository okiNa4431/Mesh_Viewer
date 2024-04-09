#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>

using namespace std;
using readingFunc = function<bool(const string&,vector<unsigned char>&, vector<unsigned int>&)>;

class io
{
private:
	map<string, readingFunc> _ext2readFunc;
	//シングルトン
	io();
	~io();
	io(const io&) = delete;
	void operator=(const io&) = delete;
	//以下、メッシュの読み込み関数
	bool Read_ply(const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices);

public:
	static io& Instance();
	void LoadMesh(const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices);
};
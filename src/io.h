#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>

using namespace std;
using readingFunc = function<bool(const string&,vector<unsigned char>&, vector<unsigned int>&)>;

class io
{
private:
	map<string, readingFunc> _ext2readFunc;
	//ƒVƒ“ƒOƒ‹ƒgƒ“
	io();
	~io();
	io(const io&) = delete;
	void operator=(const io&) = delete;

public:
	static io& Instance();
	void LoadMesh(const string& filePath, vector<unsigned char>& vertices, vector<unsigned int>& indices);
};
#include "io.h"

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
	printf("Reading PLY File.\n");
	return true;
}

io::io()
{
	_ext2readFunc["ply"] = Read_ply;
}
io::~io()
{
}
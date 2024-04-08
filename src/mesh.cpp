#include"mesh.h"
#include "io.h"

mesh::mesh(const string& filePath)
{
	LoadMesh(filePath);

	if (!SUCCEEDED(CreateVertexBuffer())) return;
	if (!SUCCEEDED(CreateIndexBuffer())) return;
	if (!SUCCEEDED(CreateVertexView())) return;
	if (!SUCCEEDED(CreateIndexView())) return;
}
mesh::~mesh()
{
}

HRESULT mesh::CreateVertexBuffer()
{
	return S_OK;
}
HRESULT mesh::CreateIndexBuffer()
{
	return S_OK;
}
HRESULT mesh::CreateVertexView()
{
	return S_OK;
}
HRESULT mesh::CreateIndexView()
{
	return S_OK;
}

void mesh::LoadMesh(const string& filePath)
{
	//拡張子から適切なメッシュ読み込み用関数を呼び出し
	auto& IO = io::Instance();
	IO.LoadMesh(filePath, _vertices, _indices);
}
#include"mesh.h"

mesh::mesh(const string& filePath)
{
	LoadMesh(filePath);

	if (!CreateVertexBuffer()) return;
	if (!CreateIndexBuffer()) return;
	if (!CreateVertexView()) return;
	if (!CreateIndexView()) return;
}

mesh::~mesh()
{
}
#include"mesh.h"
#include "io.h"
#include "d3dx12.h"
#include <iostream>

mesh::mesh(const string& filePath, shared_ptr<Dx12Wrapper> dx12) :_dx12(dx12)
{
	LoadMesh(filePath);
	if (!SUCCEEDED(CreateVertexBuffer())) return;
	if (!SUCCEEDED(CreateIndexBuffer())) return;
	if (!SUCCEEDED(CreateVertexView())) return;
	if (!SUCCEEDED(CreateIndexView())) return;
	cout << filePath << " 読み込み完了" << endl;
}
mesh::~mesh()
{
}

HRESULT mesh::CreateVertexBuffer()
{
	//バッファ作成
	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC resdesc = {};
	printf("vertices: %d bytes\n", (int)_vertices.size());
	resdesc = CD3DX12_RESOURCE_DESC::Buffer(_vertices.size()*sizeof(_vertices[0]));
	auto result = _dx12->Device()->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_vBuffer));

	//データをマップ
	unsigned char* vertMap = nullptr;
	result = _vBuffer->Map(0, nullptr, (void**)&vertMap);
	std::copy(begin(_vertices), end(_vertices), vertMap);
	_vBuffer->Unmap(0, nullptr);

	return result;
}
HRESULT mesh::CreateIndexBuffer()
{
	//バッファ作成
	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC resdesc = {};
	resdesc = CD3DX12_RESOURCE_DESC::Buffer(_indices.size()*sizeof(_indices[0]));
	auto result = _dx12->Device()->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_iBuffer));
	
	//データをマップ
	unsigned int* indxMap = nullptr;
	result = _iBuffer->Map(0, nullptr, (void**)&indxMap);
	std::copy(begin(_indices), end(_indices), indxMap);
	_iBuffer->Unmap(0, nullptr);

	return result;
}
HRESULT mesh::CreateVertexView()
{
	_vbView.BufferLocation = _vBuffer->GetGPUVirtualAddress();
	_vbView.SizeInBytes = _vertices.size();
	_vbView.StrideInBytes = 12;//float3つ>4*3
	return S_OK;
}
HRESULT mesh::CreateIndexView()
{
	_ibView.BufferLocation = _iBuffer->GetGPUVirtualAddress();
	_ibView.Format = DXGI_FORMAT_R32_UINT;
	_ibView.SizeInBytes = _indices.size()*sizeof(_indices[0]);
	return S_OK;
}

void mesh::LoadMesh(const string& filePath)
{
	auto& IO = io::Instance();
	IO.LoadMesh(filePath, _vertices, _indices);
}

void mesh::Draw()
{
	auto cmdlist = _dx12->CommandList();
	//ビューとトポロジーの設定
	cmdlist->IASetVertexBuffers(0, 1, &_vbView);
	cmdlist->IASetIndexBuffer(&_ibView);
	cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画
	cmdlist->DrawIndexedInstanced(_indices.size(), 1, 0, 0, 0);
}
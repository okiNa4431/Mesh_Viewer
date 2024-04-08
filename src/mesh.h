#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include <vector>

using namespace Microsoft::WRL;
using namespace std;

class mesh
{
private:
	//���_�ƃC���f�b�N�X�̃o�b�t�@�ƃr���[
	ComPtr<ID3D12Resource> _vBuffer = nullptr;
	ComPtr<ID3D12Resource> _iBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW _vbView;
	D3D12_INDEX_BUFFER_VIEW _ibView;
		//���������֐�
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateVertexView();
	HRESULT CreateIndexView();

	//���_�ƃC���f�b�N�X�{��
	vector<unsigned char> _vertices;
	vector<unsigned int> _indices;

public:
	void LoadMesh(const string& filePath);
	mesh(const string& filePath);
	~mesh();
};
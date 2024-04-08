#pragma once
#include <wrl.h>
#include <d3d12.h>

using namespace Microsoft::WRL;

class mesh
{

private:
	//頂点バッファ、インデックスバッファ
	ComPtr<ID3D12Resource> _vBuffer;
	ComPtr<ID3D12Resource> _iBuffer;
	void LoadMesh();

};
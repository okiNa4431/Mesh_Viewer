#pragma once
#include <wrl.h>
#include <d3d12.h>

using namespace Microsoft::WRL;

class mesh
{

private:
	//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> _vBuffer;
	ComPtr<ID3D12Resource> _iBuffer;
	void LoadMesh();

};
#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>
#include <vector>
#include "mesh.h"
#include <DirectXMath.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class Dx12Wrapper;
class mesh;

class renderer
{
private:
	//�p�C�v���C���ƃ��[�g�V�O�l�`��
	HRESULT CreateSignature();
	HRESULT CreatePipeline();
	ComPtr<ID3D12PipelineState> _pls = nullptr;//�{�`�����̃p�C�v���C��
	ComPtr<ID3D12RootSignature> _rootSignature = nullptr;//�{�`�����̃��[�g�V�O�l�`��
	std::shared_ptr<Dx12Wrapper> _dx12;

	//�f�B�X�N���v�^�q�[�v
	HRESULT CreateDescHeap();
	ID3D12DescriptorHeap* _descHeap = nullptr;

	//�r���[�v���W�F�N�V������烏�[���h���W�ւ̕ϊ�
	struct SceneMat
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
	};
	HRESULT setSceneMatrix();//�s��̐ݒ�
	ComPtr<ID3D12Resource> _transformMatBuff = nullptr;
		//�ێ����Ă����p�����[�^
	float _angle = 0.0;

	//�ێ����郁�b�V��
	std::vector<std::shared_ptr<mesh>> _meshes;
public:
	renderer(std::shared_ptr<Dx12Wrapper> dx12);
	~renderer();
	void SetPipelineAndSignature();
	void Draw();
	void Update();
	void setMatData();
	void AddMesh(const std::string& filePath);
	void AddMesh(shared_ptr<mesh> mesh);
};
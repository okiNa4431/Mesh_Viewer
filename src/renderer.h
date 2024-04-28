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
		XMMATRIX rotate;
		XMMATRIX view;
		XMMATRIX proj;
		XMFLOAT3 eye;
	};
	HRESULT setSceneMatrix();//�s��̐ݒ�
	ComPtr<ID3D12Resource> _transformMatBuff = nullptr;
		//�ێ����Ă����p�����[�^
	XMFLOAT3 _eye = XMFLOAT3(0, 0, 400);
	XMFLOAT3 _target = XMFLOAT3(0, 0, 0);
	XMFLOAT3 _up = XMFLOAT3(0, 1, 0);
	XMFLOAT3 _worldPos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 _changePos = XMFLOAT3(0, 0, 0);
	float _totalDiffPosX = 0.0f;
	float _totalDiffPosY = 0.0f;
	float _lastPosX = 0.0f;
	float _lastPosY = 0.0f;
	XMFLOAT3 _rotateAngle = XMFLOAT3(0, 0, 0);

	//�ێ����郁�b�V��
	std::vector<std::shared_ptr<mesh>> _meshes;
public:
	//Application�Ńf�o�b�O�\�����邽�߂�public�ֈڍs��
	float _phi = 0.0f;
	float _changePhi = 0.0f;
	float _theta = 0.0f;
	float _changeTheta = 0.0f;

	renderer(std::shared_ptr<Dx12Wrapper> dx12);
	~renderer();
	void SetPipelineAndSignature();
	void Draw();
	void Update(int wheel, bool downMButton, bool downLButton);
	void setMatData();
	void AddMesh(const std::string& filePath);
	void AddMesh(shared_ptr<mesh> mesh);
};
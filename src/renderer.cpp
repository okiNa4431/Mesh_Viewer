#include "renderer.h"
#include <cassert>
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <array>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <iostream>
#include "Dx12Wrapper.h"
#include "Application.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;
using namespace std;

HRESULT renderer::CreateSignature()
{
	//�����W
	D3D12_DESCRIPTOR_RANGE descRange[1] = {};
	descRange[0].NumDescriptors = 1;
	descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//�r���[�v���W�F�N�V������烏�[���h���W�ւ̕ϊ����
	descRange[0].BaseShaderRegister = 0;
	descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//�e�[�u��
	D3D12_ROOT_PARAMETER rootParams[1] = {};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParams[0].DescriptorTable.pDescriptorRanges = &descRange[0];
	rootParams[0].DescriptorTable.NumDescriptorRanges = 1;

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.NumParameters = 1;
	rsDesc.pParameters = &rootParams[0];
	rsDesc.NumStaticSamplers = 0;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//���[�g�V�O�l�`���̐���
	ComPtr<ID3DBlob> rsBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	auto result = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, rsBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		assert(SUCCEEDED(result));
		return result;
	}
	result = _dx12->Device()->CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(_rootSignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		assert(SUCCEEDED(result));
		return result;
	}
	rsBlob->Release();
	return result;
}

HRESULT renderer::CreatePipeline()
{
	//HLSL
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
		//HLSL�ǂݍ���
	auto result = D3DCompileFromFile(L"C:/Users/NaokiMurakami/source/repos/MyMeshViewer/src/Shader/VertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "BasicVS", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
	if (FAILED(result))
	{
		if (result == __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			printf("Not Vertex Found File\n");
			return result;
		}
		else
		{
			string errstr;
			errstr.resize(errorBlob->GetBufferSize());
			copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
			errstr += "\n";

			printf(errstr.c_str());
		}
	}
	result = D3DCompileFromFile(L"C:/Users/NaokiMurakami/source/repos/MyMeshViewer/src/Shader/PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "BasicPS", "ps_5_0", 0, 0, &psBlob, &errorBlob);
	if (FAILED(result))
	{
		if (result == __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			printf("Not Pixel Found File\n");
			return result;
		}
		else
		{
			string errstr;
			errstr.resize(errorBlob->GetBufferSize());
			copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
			errstr += "\n";

			printf(errstr.c_str());
		}
	}

	//��U�����Ń��C�A�E�g���`����(�{���͓ǂݍ��ݎ��ɐ������Ĉ����Ƃ��Ď�������)
	D3D12_INPUT_ELEMENT_DESC _inputlayout[] = {
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//�p�C�v���C������
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
		//�V�F�[�_�[����
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState.MultisampleEnable = false;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;

	//���C�A�E�g
	gpipeline.InputLayout.pInputElementDescs = _inputlayout;
	gpipeline.InputLayout.NumElements = sizeof(_inputlayout)/sizeof(_inputlayout[0]);

	//�u�����h�X�e�[�g
	gpipeline.BlendState.AlphaToCoverageEnable = false;
	gpipeline.BlendState.IndependentBlendEnable = false;

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.LogicOpEnable = false;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

	//�`�悷��O�p�`�̐ݒ�
	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//�����_�[�^�[�Q�b�g
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	//�A���`�G�C���A�V���O
	gpipeline.SampleDesc.Count = 1;
	gpipeline.SampleDesc.Quality = 0;

	//�[�x�o�b�t�@����
	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		//���[�g�V�O�l�`��
	gpipeline.pRootSignature = _rootSignature.Get();

	//����
	result = _dx12->Device()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&_pls));
	if (FAILED(result))
	{
		printf("Error code: 0x%X\n", result);
		assert(SUCCEEDED(result));
	}
	return result;
}

HRESULT renderer::CreateDescHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	auto result = _dx12->Device()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&_descHeap));

	return result;
}

HRESULT renderer::setSceneMatrix()
{
	//�s��̐���(�����l)
	auto worMat = XMMatrixIdentity();
	auto rotateMat = XMMatrixRotationY(_angle);
		//�s��𐶐�����O��Application�N���X�o�R�ŃE�B���h�E�̃T�C�Y�����炤
	auto& app = Application::Instance();
	SIZE window = app.GetWindowSize();
	auto viewMat = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));
	auto projMat = XMMatrixPerspectiveFovLH(XM_PIDIV2, static_cast<float>(window.cx) / static_cast<float>(window.cy), 1.0f, 500.0f);

	//�萔�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC resdesc = {};
	auto buffSize = (sizeof(SceneMat) + 0xff) & ~0xff;
	resdesc = CD3DX12_RESOURCE_DESC::Buffer(buffSize);
	auto result = _dx12->Device()->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_transformMatBuff.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(SUCCEEDED(result));
		return result;
	}

	//���ۂ̒l��map
	SceneMat* mapSceneMat = nullptr;
	result = _transformMatBuff->Map(0, nullptr, (void**)&mapSceneMat);
	mapSceneMat->world = worMat;
	mapSceneMat->rotate = rotateMat;
	mapSceneMat->view = viewMat;
	mapSceneMat->proj = projMat;
	_transformMatBuff->Unmap(0, nullptr);

	//�r���[�𐶐�
	D3D12_CONSTANT_BUFFER_VIEW_DESC matBuffViewDesc = {};
	matBuffViewDesc.BufferLocation = _transformMatBuff->GetGPUVirtualAddress();
	matBuffViewDesc.SizeInBytes = buffSize;

	//�q�[�v�ɏ悹��
	_dx12->Device()->CreateConstantBufferView(&matBuffViewDesc, _descHeap->GetCPUDescriptorHandleForHeapStart());
	
	return result;
}

renderer::renderer(std::shared_ptr<Dx12Wrapper> dx12) : _dx12(dx12)
{
	assert(SUCCEEDED(CreateSignature()));
	assert(SUCCEEDED(CreatePipeline()));
	assert(SUCCEEDED(CreateDescHeap()));
	assert(SUCCEEDED(setSceneMatrix()));
}

renderer::~renderer()
{
}

void renderer::SetPipelineAndSignature()
{
	auto cmdList = _dx12->CommandList();
	cmdList->SetPipelineState(_pls.Get());
	cmdList->SetGraphicsRootSignature(_rootSignature.Get());
}

void renderer::Draw()
{
	for (auto& mesh : _meshes)
	{
		mesh->Draw();
	}
}

void renderer::Update(int wheel, bool downMButton, bool downLButton)
{
	//���͒l�ɂ���č��W�ϊ�
	//�z�C�[���̓��͒l�ɂ���Ċg��k��
	XMVECTOR t2eVecN = XMVectorSubtract(XMLoadFloat3(&_eye), XMLoadFloat3(&_target));
	XMFLOAT3 scalingFloat3;
	if ((1 << 16) / 2 < wheel) wheel = wheel - (1 << 16);//�g�厞�͓��͒l��1<<16����������l�ɂȂ�̂ŕ��̒l�ɂȂ�悤�C��
	XMStoreFloat3(&scalingFloat3, (float)wheel*XMVector3Length(t2eVecN).m128_f32[0]/1000.0 * XMVector3Normalize(t2eVecN));
	_eye.x += scalingFloat3.x; _eye.y += scalingFloat3.y; _eye.z += scalingFloat3.z;
	//�}�E�X���{�^�������Ȃ���ŕ��s�ړ�(�����ł̓}�E�X�̈ړ��ʂ����炤����)
	POINT currentMousePos;
	GetCursorPos(&currentMousePos);
	ScreenToClient(GetActiveWindow(), &currentMousePos);
	if (_lastPosX != 0 || _lastPosY != 0)
	{
		_totalDiffPosX += currentMousePos.x - _lastPosX;
		_totalDiffPosY += currentMousePos.y - _lastPosY;
	}
	_lastPosX = currentMousePos.x;
	_lastPosY = currentMousePos.y;
	//�s��𐶐�����O��Application�N���X�o�R�ŃE�B���h�E�̃T�C�Y�����炤
	auto& app = Application::Instance();
	SIZE window = app.GetWindowSize();
	auto viewMat = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));
	auto projMat = XMMatrixPerspectiveFovLH(XM_PIDIV2*2.0f/3.0f, static_cast<float>(window.cx) / static_cast<float>(window.cy), 0.1f, 10000.0f);
		//�����X�N���[�����W�ł̃}�E�X�̍������[���h���W�֕ϊ����A���[���h�s��ɕϊ�
	XMVECTOR downVector = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	downVector = XMVector3Transform(downVector, XMMatrixInverse(nullptr, projMat));
	downVector = XMVector3Transform(downVector, XMMatrixInverse(nullptr, viewMat));
	XMVECTOR rightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	rightVector = XMVector3Transform(rightVector, XMMatrixInverse(nullptr, projMat));
	rightVector = XMVector3Transform(rightVector, XMMatrixInverse(nullptr, viewMat));
	XMVECTOR eyeVec = XMLoadFloat3(&_eye);
	XMVECTOR tarVec = XMLoadFloat3(&_target);
	XMVECTOR plane = XMPlaneFromPointNormal(XMLoadFloat3(&_target), eyeVec - tarVec);
	XMVECTOR worldDownVec = XMPlaneIntersectLine(plane, eyeVec, downVector);
	XMVECTOR worldRightVec = XMPlaneIntersectLine(plane, eyeVec, rightVector);
		//���[���h��Ԃł̕��ʂ̊��x�N�g���Ƀ}�E�X�̈ړ��ʂ�������
	worldDownVec *= -_totalDiffPosY/(window.cy/2.0f);
	worldRightVec *= _totalDiffPosX/ (window.cx / 2.0f);

	//���ۂ̈ړ�
		//�}�E�X���{�^���������Ă�����ړ��ʂɔ�Ⴕ�ĕ��s�ړ�
	if (downMButton)
	{
		_changePos.x = worldDownVec.m128_f32[0] + worldRightVec.m128_f32[0];
		_changePos.y = worldDownVec.m128_f32[1] + worldRightVec.m128_f32[1];
		_changePos.z = worldDownVec.m128_f32[2] + worldRightVec.m128_f32[2];
	}
		//�}�E�X���{�^���������Ă�����ړ��ʂɔ�Ⴕ�ĉ�](���s�ړ��D��)
	else if (downLButton)
	{
		_changePhi = (_totalDiffPosX) * 0.003f;
	}
	else
	{
		//���s�ړ����̕ۑ�
		_worldPos.x += _changePos.x;
		_worldPos.y += _changePos.y;
		_worldPos.z += _changePos.z;
		_changePos = XMFLOAT3(0, 0, 0);
		//��]���̕ۑ�
		_phi += _changePhi;
		_changePhi = 0.0f;
		//�}�E�X�̈ړ����̃��Z�b�g
		_totalDiffPosX = 0.0;
		_totalDiffPosY = 0.0;
	}
	auto worMat = XMMatrixTranslation(_worldPos.x+_changePos.x, _worldPos.y + _changePos.y, _worldPos.z + _changePos.z);
	auto rotateMat = XMMatrixRotationY(_phi+_changePhi);

	//�}�b�v
	SceneMat* mapSceneMat = nullptr;
	auto result = _transformMatBuff->Map(0, nullptr, (void**)&mapSceneMat);
	mapSceneMat->world = worMat;
	mapSceneMat->rotate = rotateMat;
	mapSceneMat->view = viewMat;
	mapSceneMat->proj = projMat;
	_transformMatBuff->Unmap(0, nullptr);
}

void renderer::setMatData()
{
	auto cmdList = _dx12->CommandList();
	cmdList->SetDescriptorHeaps(1, &_descHeap);
	cmdList->SetGraphicsRootDescriptorTable(0, _descHeap->GetGPUDescriptorHandleForHeapStart());
}

void renderer::AddMesh(shared_ptr<mesh> mesh)
{
	_meshes.emplace_back(mesh);
}

void renderer::AddMesh(const std::string& filePath)
{
	AddMesh(make_shared<mesh>(filePath, _dx12));
}
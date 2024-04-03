#include "renderer.h"
#include <cassert>
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <array>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "Dx12Wrapper.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace std;

bool renderer::CreateSignature()
{
	//�����W
	CD3DX12_DESCRIPTOR_RANGE descRange[1] = {};
	descRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);//�r���[�v���W�F�N�V������烏�[���h���W�ւ̕ϊ����
	//�e�[�u��
	CD3DX12_ROOT_PARAMETER rootParams[1] = {};
	rootParams[0].InitAsDescriptorTable(1, &descRange[0]);

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.NumParameters = 1;
	rsDesc.pParameters = &rootParams[0];
	rsDesc.NumStaticSamplers = 0;
	//���[�g�V�O�l�`���̐���
	ComPtr<ID3DBlob> rsBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	auto result = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, rsBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		assert(SUCCEEDED(result));
		return result;
	}
	result = _dx12.Device()->CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(_rootSignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		assert(SUCCEEDED(result));
		return result;
	}

	return result;
}

bool renderer::CreatePipeline()
{
	//HLSL
	ComPtr<ID3DBlob> vsBlob = nullptr;
	ComPtr<ID3DBlob> psBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
		//HLSL�ǂݍ���
	auto result = D3DCompileFromFile(L"BasicVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "BasicVS", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, vsBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		if (result == __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			printf("Not Found File\n");
			return 0;
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
	result = D3DCompileFromFile(L"BasicPixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "BasicPS", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, psBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		if (result == __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			printf("Not Found File\n");
			return 0;
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
	D3D12_INPUT_ELEMENT_DESC _inputlayout[1]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	//�p�C�v���C������
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpDesc = {};
		//�V�F�[�_�[����
	gpDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
		//�O�p�`���
	gpDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	gpDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpDesc.SampleDesc.Count = 1;
	gpDesc.SampleDesc.Quality = 0;
	gpDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	gpDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		//���C�A�E�g
	gpDesc.InputLayout.pInputElementDescs = _inputlayout;
	gpDesc.InputLayout.NumElements = sizeof(_inputlayout)/sizeof(_inputlayout[0]);
		//�u�����h�X�e�[�g
	gpDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		//�����_�[�^�[�Q�b�g
	gpDesc.NumRenderTargets = 1;
	gpDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		//�[�x�o�b�t�@����
	gpDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		//���[�g�V�O�l�`��
	gpDesc.pRootSignature = _rootSignature.Get();
	
	//����
	auto result = _dx12.Device()->CreateGraphicsPipelineState(&gpDesc, IID_PPV_ARGS(_pls.ReleaseAndGetAddressOf()));
	if (FAILED(result)) assert(SUCCEEDED(result));
	return result;
}

renderer::renderer(Dx12Wrapper dx12) :_dx12(dx12)
{
	assert(SUCCEEDED(CreateSignature()));
	assert(SUCCEEDED(CreatePipeline()));
}

renderer::~renderer()
{
}

void renderer::Draw()
{

}
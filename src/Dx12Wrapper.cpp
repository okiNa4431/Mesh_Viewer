#include <Windows.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "Dx12Wrapper.h"
#include "Application.h"
#include <vector>
#include <string>
#include <d3dx12.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

namespace
{
	void EnableDebugLayer()
	{
		ID3D12Debug* debugLayer = nullptr;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer))))
		{
			debugLayer->EnableDebugLayer();
			debugLayer->Release();
		}
	}
}

Dx12Wrapper::Dx12Wrapper(HWND hwnd)
{
#ifdef _DEBUG
	EnableDebugLayer();
#endif
	auto& app = Application::Instance();
	_winSize = app.GetWindowSize();

	//DirectX12�֘A������
	if (FAILED(InitializeDXGIDevice())) {
		assert(0);
		return;
	}
	if (FAILED(InitializeCommand())) {
		assert(0);
		return;
	}
	if (FAILED(CreateSwapChain(hwnd))) {
		assert(0);
		return;
	}
	if (FAILED(CreateFinalRenderTargets())) {
		assert(0);
		return;
	}

	//�[�x�o�b�t�@�쐬
	if (FAILED(CreateDepthStencilView())) {
		assert(0);
		return;
	}

	//�t�F���X�̍쐬
	if (FAILED(_dev->CreateFence(_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.ReleaseAndGetAddressOf())))) {
		assert(0);
		return;
	}
}
Dx12Wrapper::~Dx12Wrapper()
{
}

HRESULT Dx12Wrapper::InitializeDXGIDevice()
{
	//�A�_�v�^�̐���
#ifdef _DEBUG
	auto result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(_dxgiFactory.ReleaseAndGetAddressOf()));
#else
	auto result = CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory));
#endif//_DEBUG
	vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; _dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		adapters.push_back(tmpAdapter);
	}

	for (auto adpt : adapters)
	{
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);

		wstring strDesc = adesc.Description;

		if (strDesc.find(L"NVIDIA") != string::npos)
		{
			tmpAdapter = adpt;
			break;
		}
	}

	//�f�o�C�X�̐���
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	D3D_FEATURE_LEVEL featureLevel;
	for (auto lv : levels)
	{
		if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(_dev.ReleaseAndGetAddressOf())) == S_OK)
		{
			featureLevel = lv;
			break;
		}
	}

	return result;
}

HRESULT Dx12Wrapper::InitializeCommand()
{
	auto result = _dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_cmdAllocator.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}
	result = _dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAllocator.Get(), nullptr, IID_PPV_ARGS(_cmdList.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	result = _dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(_cmdQueue.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
	return result;
}

HRESULT Dx12Wrapper::CreateSwapChain(const HWND& hwnd)
{
	//�X���b�v�`�F�[��
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = _winSize.cx;
	swapchainDesc.Height = _winSize.cy;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	auto result = _dxgiFactory->CreateSwapChainForHwnd(_cmdQueue.Get(), hwnd, &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)_swapchain.ReleaseAndGetAddressOf());

	return result;
}

HRESULT Dx12Wrapper::CreateDepthStencilView()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	auto result = _swapchain->GetDesc1(&desc);
	//�[�x�o�b�t�@�쐬
	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resdesc.DepthOrArraySize = 1;
	resdesc.Width = desc.Width;
	resdesc.Height = desc.Height;
	resdesc.Format = DXGI_FORMAT_D32_FLOAT;
	resdesc.SampleDesc.Count = 1;
	resdesc.SampleDesc.Quality = 0;
	resdesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resdesc.MipLevels = 1;
	resdesc.Alignment = 0;

	//�f�v�X�p�q�[�v�v���p�e�B
	auto depthHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE depthClearValue(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = _dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //�f�v�X�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(_depthBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		//�G���[����
		return result;
	}

	//�[�x�̂��߂̃f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};//�[�x�Ɏg����Ƃ��������킩��΂���
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[1�̂�
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[�Ƃ��Ďg��
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;


	result = _dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(_dsvHeap.ReleaseAndGetAddressOf()));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�f�v�X�l��32bit�g�p
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;//�t���O�͓��ɂȂ�
	_dev->CreateDepthStencilView(_depthBuffer.Get(), &dsvDesc, _dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

HRESULT	Dx12Wrapper::CreateFinalRenderTargets()
{
	//�E�B���h�E�T�C�Y�̂��߂ɃX���b�v�`�F�[���𓾂�
	DXGI_SWAP_CHAIN_DESC1 swcDesc = {};
	auto result = _swapchain->GetDesc1(&swcDesc);

	//�f�B�X�N���v�^�q�[�v(�����_�[�^�[�Q�b�g�r���[�p)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	result = _dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(_rtvHeaps.ReleaseAndGetAddressOf()));
	if (result == S_OK) printf("Created Descriptor Heap.\n");

	_backBuffers.resize(swcDesc.BufferCount);

	D3D12_CPU_DESCRIPTOR_HANDLE handle = _rtvHeaps->GetCPUDescriptorHandleForHeapStart();

	//SRGB�����_�[�^�[�Q�b�g�r���[�ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (int i = 0; i < swcDesc.BufferCount; ++i) {
		result = _swapchain->GetBuffer(i, IID_PPV_ARGS(&_backBuffers[i]));
		assert(SUCCEEDED(result));
		rtvDesc.Format = _backBuffers[i]->GetDesc().Format;
		_dev->CreateRenderTargetView(_backBuffers[i], &rtvDesc, handle);
		handle.ptr += _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	_viewport.reset(new CD3DX12_VIEWPORT(_backBuffers[0]));
	_scissorrect.reset(new CD3DX12_RECT(0, 0, swcDesc.Width, swcDesc.Height));
	return result;
}

void
Dx12Wrapper::BeginDraw() {
	//DirectX����
	//�o�b�N�o�b�t�@�̃C���f�b�N�X���擾
	auto bbIdx = _swapchain->GetCurrentBackBufferIndex();
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_backBuffers[bbIdx],
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	_cmdList->ResourceBarrier(1, &barrier);


	//�����_�[�^�[�Q�b�g���w��
	auto rtvH = _rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIdx * _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//�[�x���w��
	auto dsvH = _dsvHeap->GetCPUDescriptorHandleForHeapStart();
	_cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	//��ʃN���A
	float clearColor[] = { 1.0f,1.0f,1.0f,1.0f };//���F
	_cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	//�r���[�|�[�g�A�V�U�[��`�̃Z�b�g
	_cmdList->RSSetViewports(1, _viewport.get());
	_cmdList->RSSetScissorRects(1, _scissorrect.get());
}

void Dx12Wrapper::Update() {

}

void Dx12Wrapper::EndDraw() {
	auto bbIdx = _swapchain->GetCurrentBackBufferIndex();

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_backBuffers[bbIdx], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	_cmdList->ResourceBarrier(1,
		&barrier);

	//���߂̃N���[�Y
	_cmdList->Close();

	//�R�}���h���X�g�̎��s
	ID3D12CommandList* cmdlists[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(1, cmdlists);
	////�҂�
	_cmdQueue->Signal(_fence.Get(), ++_fenceVal);

	if (_fence->GetCompletedValue() < _fenceVal) {
		auto event = CreateEvent(nullptr, false, false, nullptr);
		_fence->SetEventOnCompletion(_fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	_cmdAllocator->Reset();//�L���[���N���A
	_cmdList->Reset(_cmdAllocator.Get(), nullptr);//�ĂуR�}���h���X�g�����߂鏀��
}

ComPtr<ID3D12Device> Dx12Wrapper::Device() {
	return _dev;
}
ComPtr<ID3D12GraphicsCommandList> Dx12Wrapper::CommandList() {
	return _cmdList;
}
ComPtr<IDXGISwapChain4> Dx12Wrapper::Swapchain() {
	return _swapchain;
}
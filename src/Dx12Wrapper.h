#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<memory>
#include<vector>
using namespace Microsoft::WRL;

class Dx12Wrapper
{
	SIZE _winSize;
	
	//DXGI�܂��
	ComPtr<IDXGIFactory4> _dxgiFactory = nullptr;//DXGI�C���^�[�t�F�C�X
	ComPtr<IDXGISwapChain4> _swapchain = nullptr;//�X���b�v�`�F�C��

	//DirectX12�܂��
	ComPtr<ID3D12Device> _dev = nullptr;//�f�o�C�X
	ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr;//�R�}���h�A���P�[�^
	ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr;//�R�}���h���X�g
	ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr;//�R�}���h�L���[

	//�\���Ɋւ��o�b�t�@����
	ComPtr<ID3D12Resource> _depthBuffer = nullptr;//�[�x�o�b�t�@
	std::vector<ID3D12Resource*> _backBuffers;//�o�b�N�o�b�t�@(2�ȏ�c�X���b�v�`�F�C�����m��)
	ComPtr<ID3D12DescriptorHeap> _rtvHeaps = nullptr;//�����_�[�^�[�Q�b�g�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr;//�[�x�o�b�t�@�r���[�p�f�X�N���v�^�q�[�v
	std::unique_ptr<D3D12_VIEWPORT> _viewport;//�r���[�|�[�g
	std::unique_ptr<D3D12_RECT> _scissorrect;//�V�U�[��`

	//�t�F���X
	ComPtr<ID3D12Fence> _fence = nullptr;
	UINT64 _fenceVal = 0;

	//�ŏI�I�ȃ����_�[�^�[�Q�b�g�̐���
	HRESULT	CreateFinalRenderTargets();

	//�f�v�X�X�e���V���r���[�̐���
	HRESULT CreateDepthStencilView();

	//�X���b�v�`�F�C���̐���
	HRESULT CreateSwapChain(const HWND& hwnd);

	//DXGI�܂�菉����
	HRESULT InitializeDXGIDevice();

	//�R�}���h�܂�菉����
	HRESULT InitializeCommand();

public:
	Dx12Wrapper(HWND hwnd);
	~Dx12Wrapper();

	void Update();
	void BeginDraw();
	void EndDraw();

	ID3D12Device* Device();//�f�o�C�X
	ComPtr < ID3D12GraphicsCommandList> CommandList();//�R�}���h���X�g
	ComPtr < IDXGISwapChain4> Swapchain();//�X���b�v�`�F�C��
};
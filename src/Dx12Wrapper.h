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
	
	//DXGIまわり
	ComPtr<IDXGIFactory4> _dxgiFactory = nullptr;//DXGIインターフェイス
	ComPtr<IDXGISwapChain4> _swapchain = nullptr;//スワップチェイン

	//DirectX12まわり
	ComPtr<ID3D12Device> _dev = nullptr;//デバイス
	ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr;//コマンドアロケータ
	ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr;//コマンドリスト
	ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr;//コマンドキュー

	//表示に関わるバッファ周り
	ComPtr<ID3D12Resource> _depthBuffer = nullptr;//深度バッファ
	std::vector<ID3D12Resource*> _backBuffers;//バックバッファ(2つ以上…スワップチェインが確保)
	ComPtr<ID3D12DescriptorHeap> _rtvHeaps = nullptr;//レンダーターゲット用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr;//深度バッファビュー用デスクリプタヒープ
	std::unique_ptr<D3D12_VIEWPORT> _viewport;//ビューポート
	std::unique_ptr<D3D12_RECT> _scissorrect;//シザー矩形

	//フェンス
	ComPtr<ID3D12Fence> _fence = nullptr;
	UINT64 _fenceVal = 0;

	//最終的なレンダーターゲットの生成
	HRESULT	CreateFinalRenderTargets();

	//デプスステンシルビューの生成
	HRESULT CreateDepthStencilView();

	//スワップチェインの生成
	HRESULT CreateSwapChain(const HWND& hwnd);

	//DXGIまわり初期化
	HRESULT InitializeDXGIDevice();

	//コマンドまわり初期化
	HRESULT InitializeCommand();

public:
	Dx12Wrapper(HWND hwnd);
	~Dx12Wrapper();

	void Update();
	void BeginDraw();
	void EndDraw();

	ID3D12Device* Device();//デバイス
	ComPtr < ID3D12GraphicsCommandList> CommandList();//コマンドリスト
	ComPtr < IDXGISwapChain4> Swapchain();//スワップチェイン
};
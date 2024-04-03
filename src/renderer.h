#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>

using namespace Microsoft::WRL;

class Dx12Wrapper;

class renderer
{
private:
	bool CreateSignature();
	bool CreatePipeline();
	ComPtr<ID3D12PipelineState> _pls = nullptr;//本チャンのパイプライン
	ComPtr<ID3D12RootSignature> _rootSignature = nullptr;//本チャンのルートシグネチャ
	Dx12Wrapper& _dx12;

public:
	renderer(Dx12Wrapper dx12);
	~renderer();
	void Draw();
};
#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>

using namespace Microsoft::WRL::ComPtr;

class renderer
{
private:
	bool CreateSignature();
	bool CreatePipeline();
	ComPtr<ID3D12PipelineState> _pls = nullptr;//本チャンのパイプライン
	ComPtr<ID3D12RootSignature> _rootSignature = nullptr;

public:
	renderer();
	~renderer();
	void Draw();
};
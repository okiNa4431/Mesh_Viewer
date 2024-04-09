#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>
#include <vector>
#include "mesh.h"

using namespace Microsoft::WRL;

class Dx12Wrapper;
class mesh;

class renderer
{
private:
	HRESULT CreateSignature();
	HRESULT CreatePipeline();
	ComPtr<ID3D12PipelineState> _pls = nullptr;//本チャンのパイプライン
	ComPtr<ID3D12RootSignature> _rootSignature = nullptr;//本チャンのルートシグネチャ
	std::shared_ptr<Dx12Wrapper> _dx12;

	//保持するメッシュ
	std::vector<std::shared_ptr<mesh>> _meshes;
public:
	renderer(std::shared_ptr<Dx12Wrapper> dx12);
	~renderer();
	void SetPipelineAndSignature();
	void Draw();
	void AddMesh(const std::string& filePath);
	void AddMesh(shared_ptr<mesh> mesh);
};
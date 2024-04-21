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
	//パイプラインとルートシグネチャ
	HRESULT CreateSignature();
	HRESULT CreatePipeline();
	ComPtr<ID3D12PipelineState> _pls = nullptr;//本チャンのパイプライン
	ComPtr<ID3D12RootSignature> _rootSignature = nullptr;//本チャンのルートシグネチャ
	std::shared_ptr<Dx12Wrapper> _dx12;

	//ディスクリプタヒープ
	HRESULT CreateDescHeap();
	ID3D12DescriptorHeap* _descHeap = nullptr;

	//ビュープロジェクションやらワールド座標への変換
	struct SceneMat
	{
		XMMATRIX world;
		XMMATRIX rotate;
		XMMATRIX view;
		XMMATRIX proj;
	};
	HRESULT setSceneMatrix();//行列の設定
	ComPtr<ID3D12Resource> _transformMatBuff = nullptr;
		//保持しておくパラメータ
	float _angle = 0.0;
	XMFLOAT3 _eye = XMFLOAT3(400, 400, 400);
	XMFLOAT3 _target = XMFLOAT3(0, 0, 0);
	XMFLOAT3 _up = XMFLOAT3(0, 1, 0);
	float _lastPosX = 0.0f;
	float _lastPosY = 0.0f;
	
		//入力値
	int _wheel = 0;

	//保持するメッシュ
	std::vector<std::shared_ptr<mesh>> _meshes;
public:
	float _totalDiffPosX = 0.0f;
	float _totalDiffPosY = 0.0f;

	renderer(std::shared_ptr<Dx12Wrapper> dx12);
	~renderer();
	void SetPipelineAndSignature();
	void Draw();
	void Update(HWND& hwnd);
	void setMatData();
	void setInputData(int& wheel);
	void AddMesh(const std::string& filePath);
	void AddMesh(shared_ptr<mesh> mesh);
};
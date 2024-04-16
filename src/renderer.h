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
		XMMATRIX view;
		XMMATRIX proj;
	};
	HRESULT setSceneMatrix();//行列の設定
	ComPtr<ID3D12Resource> _transformMatBuff = nullptr;
		//保持しておくパラメータ
	float _angle = 0.0;
	XMFLOAT3 _eye = XMFLOAT3(0, 0, 400);
	XMFLOAT3 _target = XMFLOAT3(0, 0, 0);
	XMFLOAT3 _up = XMFLOAT3(0, 1, 0);
		//入力値
	int _wheel = 0;
	float _mouseMoveX = 0;
	float _mouseMoveY = 0;

	//保持するメッシュ
	std::vector<std::shared_ptr<mesh>> _meshes;
public:
	renderer(std::shared_ptr<Dx12Wrapper> dx12);
	~renderer();
	void SetPipelineAndSignature();
	void Draw();
	void Update();
	void setMatData();
	void setInputData(int& wheel, int& x, int& y);
	void AddMesh(const std::string& filePath);
	void AddMesh(shared_ptr<mesh> mesh);
};
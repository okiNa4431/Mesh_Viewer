#include "renderer.h"
#include <d3d12.h>

bool renderer::CreatePipeline(const D3D12_INPUT_ELEMENT_DESC& inputlayout[])
{
	ComPtr<ID3DBlob> vsBlob = nullptr;
	ComPtr<ID3DBlob> psBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	auto result = 
}
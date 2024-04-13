#include "ShaderHeader.hlsli"

float4 BasicPS(v2f i) : SV_TARGET
{
	return float4(0.2, 0.2, 0.8, 1.0);
}
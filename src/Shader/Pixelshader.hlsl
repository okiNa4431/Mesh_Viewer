#include "ShaderHeader.hlsli"

float4 BasicPS(v2f i) : SV_TARGET
{
	float3 light = normalize(float3(1000,-1000,1000));
	float brightness = saturate(dot(-light, i.normal));
	return float4(brightness, brightness, brightness, 1.0)+0.2;
}
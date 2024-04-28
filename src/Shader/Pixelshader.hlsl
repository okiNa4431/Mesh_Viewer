#include "ShaderHeader.hlsli"

float4 BasicPS(v2f i) : SV_TARGET
{
	float3 light = normalize(float3(1,0,-1));
	float brightness = saturate(dot(-light, i.normal));
	float3 vector_from_eye_to_frag = normalize(i.wPos.xyz-eye);
	float brightness_view = saturate(dot(-vector_from_eye_to_frag, i.normal));
	brightness = lerp(0.2, 0.9, brightness);
	//brightness += lerp(0.2, 0.4, brightness_view);
	return float4(brightness, brightness, brightness, 1.0)+0.1;
}
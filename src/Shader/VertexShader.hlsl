#include "ShaderHeader.hlsli"

v2f BasicVS(float4 pos : POSITION)
{
	v2f o;
	o.posSV = pos;
	return o;
}
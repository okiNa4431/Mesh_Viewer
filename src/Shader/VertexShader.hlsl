#include "ShaderHeader.hlsli"

v2f BasicVS(float4 pos : POSITION)
{
	v2f o;
	pos = mul(world, pos);
	pos = mul(mul(proj, view), pos);
	o.posSV = pos;
	return o;
}
#include "ShaderHeader.hlsli"

v2f BasicVS(float4 pos : POSITION, float4 normal : NORMAL)
{
	v2f o;
	pos = mul(rotate, pos);
	pos = mul(world, pos);
	pos = mul(mul(proj, view), pos);
	o.posSV = pos;
	o.normal = mul(rotate, normal);
	return o;
}
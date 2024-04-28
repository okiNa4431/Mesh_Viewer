struct v2f
{
	float4 posSV: SV_POSITION;
	float4 wPos : POSITION0;
	float4 normal : NORMAL0;
	float4 vnormal : NORMAL1;
};

cbuffer cbuff0 : register(b0)
{
	matrix world;
	matrix rotate;
	matrix view;
	matrix proj;
	float3 eye;
};
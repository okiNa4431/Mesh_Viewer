struct v2f
{
	float4 posSV: SV_POSITION;
	float4 pos : POSITION;
};

cbuffer cbuff0 : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
	float3 eye;
};
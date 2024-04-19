struct v2f
{
	float4 posSV: SV_POSITION;
	float4 normal : NORMAL;
};

cbuffer cbuff0 : register(b0)
{
	matrix world;
	matrix rotate;
	matrix view;
	matrix proj;
};
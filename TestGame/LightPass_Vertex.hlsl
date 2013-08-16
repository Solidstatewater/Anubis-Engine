cbuffer MatrixBuffer	:	register( c0 )
{
	float4x4 WVP;
};

////////////////////////////////////////
//shaders input structures
////////////////////////////////////////
struct vs_input
{
	float3 pos		: POSITION;
	float2 texCoord : TEXCOORDS;
};

float4 VS( vs_input input ) : SV_POSITION
{
	return mul(float4(input.pos, 1.0f), WVP);
}
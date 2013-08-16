cbuffer MatrixBuffer : register( b0 )
{
	matrix WorldView;
	matrix WVP;
};

struct ps_input
{
	float4 pos : SV_POSITION;
	float3 lightVec : TEXCOORD0;
};

float4 VarianceShadows_DepthVS( float3 pos : POSITION )
{
	ps_input output;

	output.pos = mul(float4(pos, 1.0f), WVP);
	output.lightVec = mul(float4(pos, 1.0f), WorldView).xyz;

	return output;
}
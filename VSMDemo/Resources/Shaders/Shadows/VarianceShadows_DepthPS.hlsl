cbuffer LightProperties : register( b0 )
{
	float4 lightFrustumDepth;
}
struct ps_input
{
	float4 pos	:	SV_POSITION;
	float3 lightVec : TEXCOORD0;
};

float4 VarianceShadows_DepthPS( ps_input input ) : SV_TARGET
{
	float4 res;

	//calculate depth
	float depth = length(input.lightVec) / lightFrustumDepth.x;

	//return the result
	res.x = depth;
	res.y = depth * depth;
	res.w = 1.0f;

	return res;
}

/////////////////////////////////////////
//Constant buffers
/////////////////////////////////////////
cbuffer LightProperties : register( b0 )
{
	float4 lightFrustumDepth; //store in x register
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
	//float depth = input.pos.z / 28.0f;// / lightFrustumDepth.x;
	float depth = length(input.lightVec) / lightFrustumDepth.x;

	//return the result
	res.x = depth;
	res.y = depth * depth;
	res.w = 1.0f;

	return res;
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
}
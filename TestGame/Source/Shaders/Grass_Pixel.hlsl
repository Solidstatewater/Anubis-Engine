struct ps_input
{
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
	float2 texCoords : TEXCOORD0;
};

struct ps_output
{
	float4 position			:	SV_Target0;
	float4 normal			:	SV_Target1;
	float4 depth			:	SV_Target2;
	float4 diffuseAlbedo	:	SV_Target3;
	float4 specAlbedo		:	SV_Target4;
	float4 geometryNormal	:	SV_Target5;
};

/////////////////////////////////////////////
//Texture Resources
/////////////////////////////////////////////
Texture2D	diffuseTexture	: register( t0 );
Texture2D	maskTexture		: register( t1 );
Texture2D	specularTexture : register( t2 );
SamplerState linearSampler;

/////////////////////////////////////////////
//Constant Buffers
/////////////////////////////////////////////

ps_output Grass_PS(ps_input input) : SV_TARGET
{
	ps_output output;

	float4 alphaFactor = 1.0f - maskTexture.Sample(linearSampler, input.texCoords);
	//write results to the structure
	output.position = float4(input.posWS, 1.0f);
	output.normal = float4(input.normal, 0.0f);
	output.diffuseAlbedo = diffuseTexture.Sample(linearSampler, input.texCoords) * alphaFactor;
	output.specAlbedo = specularTexture.Sample(lionearSampler, input.texCoords) * alphaFactor;

	return output;
}
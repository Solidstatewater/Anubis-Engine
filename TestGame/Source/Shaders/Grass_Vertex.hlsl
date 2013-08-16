
struct vs_input
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 texCoords : TEXCOORD0;

	//

	//float3 objectPosition : TEXCOORD1;
};

struct ps_input
{
	float4 position : SV_POSITION;
	float3 posWS	: POSITIONWS;
	float3 normal	: NORMAL;
	float2 texCoords : TEXCOORD0;
};

/////////////////////////////////////////////
//Constant Buffers
/////////////////////////////////////////////
cbuffer MatrixBuffer : register( c0 )
{
	matrix World;
	matrix WVP;
};

cbuffer WindBuffer : register( c1 )
{
	float3 windDirection;
	float windStrength;
};

cbuffer TimeBuffer : register( c2 )
{
	float4 TimeStamp;
};

ps_input Grass_VS( vs_input input )
{
	ps_input output;

	//Animate only upper vertices
	if (input.texCoords.v <= 0.1f)
	{
	}

	output.position = mul(float4(input.position, 1.0f), WVP);
	output.posWS = mul(float4(input.position, 1.0f), World);
	output.normal = mul(input.normal, (float3x3)World);
	output.texCoords = input.texCoords;

	return output;
}
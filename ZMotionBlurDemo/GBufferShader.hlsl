/*************************************************
==			Resource Declarations				==
*************************************************/

/*
	== Constant Buffers
*/

cbuffer MatrixBuffer
{
	float4x4 World;	//World Transform
	float4x4 WorldView; //World * View transform
	float4x4 WorldViewProjection; // World * View * Projection
};

cbuffer CameraBuffer
{
	float4 cameraPos;
};


/*
	== Textures and Sampler States
*/
Texture2D		diffuseTexture		:	register( t0 );
Texture2D		normalMap			:	register( t1 );
Texture2D		specularMap			:	register( t2 );
Texture2D		heightMap			:	register( t3 );
SamplerState	anisotropicSampler	:	register( s0 );


/*************************************************
==			Shader Input Structures				==
*************************************************/

/*
	== Vertex Shader Input
*/
struct vs_input
{
	float3	pos			:	POSITION;
	float2	texCoords	:	TEXCOORDS;
	float3	normal		:	NORMAL;
	float3	tangent		:	TANGENT;
	float3	binormal	:	BINORMAL;
};

/* 
	== Pixel Shader Input
*/
struct ps_input
{
	float4 posCS		:	SV_POSITION;
	float2 texCoords	:	TEXCOORDS;
	float3 normalWS		:	NORMALWS;
	float3 tangentWS	:	TANGENTWS;
	float3 binormalWS	:	BINORMALWS;
	float4 posWS		:	POSITIONWS;
	float4 posVS		:	POSITIONVS;
};

/*
	== Pixel Shader Output
*/
struct ps_output
{
	float4 position			:	SV_Target0;
	float4 normal			:	SV_Target1;
	float4 depth			:	SV_Target2;
	float4 diffuseAlbedo	:	SV_Target3;
	float4 specAlbedo		:	SV_Target4;
	float4 geometryNormal	:	SV_Target5;
};

/*************************************************
==			Vertex Shader						==
*************************************************/
ps_input	VS( vs_input input )
{
	ps_input output;

	//calculate clip-space position
	output.posCS = mul(float4(input.pos, 1.0f), WorldViewProjection);

	//pass texture coordinates
	output.texCoords = input.texCoords;

	//calculte normal and position in world space
	float3 normalWS = normalize(mul(input.normal, (float3x3)World));

	//calculate tangent vectors in the world space
	float3 tangentWS = normalize(mul(input.tangent, (float3x3)World));

	//assign tangent space
	output.normalWS = normalWS;
	output.tangentWS = tangentWS;
	output.binormalWS = normalize(cross(normalWS, tangentWS));

	//calculate position
	output.posWS  = mul(float4(input.pos, 1.0f), World);
	output.posVS  = mul(float4(input.pos, 1.0f), WorldView);

	return output;
};

/*************************************************
==			Pixel Shader						==
*************************************************/
ps_output	PS( ps_input input )
{
	const float MAX_DISTANCE = 500.0f;

	ps_output output;

	//get diffuse color
	float3 diffuse = diffuseTexture.Sample(anisotropicSampler, input.texCoords).rgb;
	float3 specular = specularMap.Sample(anisotropicSampler, input.texCoords).rgb;

	//handle tangent space and normal mapping
	float3 normalWS		= normalize(input.normalWS);
	float3 tangentWS	= normalize(input.tangentWS);
	float3 binormalWS	= normalize(input.binormalWS);
	float3x3 TangentToWorldSpace = float3x3(	(float3)tangentWS,
												(float3)binormalWS,
												(float3)normalWS	);

	float3 normalTS	= normalMap.Sample(anisotropicSampler, input.texCoords).rgb;
	float3 bumpNormalWS = normalize( normalTS * 2.0f - 1.0f);
	bumpNormalWS = mul(bumpNormalWS, TangentToWorldSpace); 
	bumpNormalWS = normalize(bumpNormalWS);

	//handle depth
	//float depth = input.posCS.z / input.posCS.w;
	float depth = (input.posVS.z - cameraPos.z) / MAX_DISTANCE;
	//float depth = input.posCS.z;

	//fill the g-buffer
	//output.position			= float4(input.posWS.x, input.posWS.y, input.posWS.z, 1.0f);
	output.position = input.posWS;
	output.normal			= float4(bumpNormalWS, 0.0f);
	//output.normal = float4(normalWS, 0.0f);
	//output.depth			= float4(depth, depth, depth, depth);
	//output.depth = float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.depth = float4(depth, depth, depth, 1.0f);
	output.diffuseAlbedo	= float4(diffuse, 1.0f);
	if (diffuse.x == 0 && diffuse.y == 0 && diffuse.z == 0)
	{
		output.diffuseAlbedo = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	//output.specAlbedo		= float4(specAlbedo, specPower);
	output.specAlbedo		= float4(specular, 1.0f);
	output.geometryNormal	= float4(normalWS, 1.0f);

	return output;
}
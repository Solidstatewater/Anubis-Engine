Texture2D	positionTexture	:	register( t0 );
Texture2D	normalTexture	:	register( t1 );
Texture2D	depthTexture	:	register( t2 );
Texture2D	diffuseTexture	:	register( t3 );
Texture2D	specularTexture	:	register( t4 );

cbuffer MatrixBuffer	:	register( c0 )
{
	float4x4 WVP;
};

cbuffer CameraBuffer	:	register( c1 )
{
	float3 cameraPos;
	float  cameraPad;
};

//Containes variables with general light properties
cbuffer LightBuffer	:	register( c2 )
{
	//Light Color
	float3 lightColor;
	float  lightColorPad;

	//Light Position
	float3 lightPos;
	float  lightPosPad;

	//Light Direction
	float3 lightDir;
	float  lightType;
};

cbuffer PointLightBuffer	:	register( c3 )
{
	//Light Range
	float4 lightRange;
};

cbuffer SpotLightBuffer			:	register( c4 )
{
	//Conus Angles
	float2 spotLightAngles;
	float2 slAnglesPad;
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

//////////////////////////////////////////
//Help methods
//////////////////////////////////////////
bool POINT_LIGHT()
{
	if (lightType == 1.0f)
		return true;

	return false;
}

bool SPOT_LIGHT()
{
	if (lightType == 2.0f)
		return true;

	return false;
}

bool DIRECTIONAL_LIGHT()
{
	if (lightType == 3.0f)
		return true;

	return false;
}

void GetGBufferAtributes( in float2 screenPos, out float3 position, out float3 normal, out float depth,
						 out float3 diffuse, out float3 specular, out float specPower )
{
	int3 sampleCoords = int3( screenPos, 0 );

	position	=	positionTexture.Load(sampleCoords).xyz;
	normal		=	normalTexture.Load(sampleCoords).xyz;
	depth		=	depthTexture.Load(sampleCoords).x;
	diffuse		=	diffuseTexture.Load(sampleCoords).xyz;

	float4 spec	=	specularTexture.Load(sampleCoords);
	specular	=	spec.xyz;
	specPower	=	spec.w;
}

float3 CalcLightning( in float3 position,
					  in float3	normal,
					  in float	depth,
					  in float3	diffuse,
					  in float3 specular,
					  in float	specPower )
{
	float3 lightPos1 = float3(0.0f, 0.0f, -3.0f);
	//firstly handle diffuse term
	float3 L = (float3)0.0f;
	float atten = 1.0f;

	//if( POINT_LIGHT() || SPOT_LIGHT() )
	{
		//calc inverse light direction
		L = lightPos1 - position;

		//calculate attenuation
		float distance = length(L);
		//normalize
		L /= distance;
		atten = max(0.0f, 1.0f - (distance / lightRange.x));
	}
	//else if( DIRECTIONAL_LIGHT() )
	//{
	//	L = -lightDir;
	//}
	

	/*if( SPOT_LIGHT() )
	{
		//dd attenuation for a spot light
		float3 L2 = lightDir;
		L = normalize(L);
		L2 = normalize(L2);
		float angle = acos(dot(-L, L2));
		atten *= ( (angle - spotLightAngles.y) /(spotLightAngles.x - spotLightAngles.y) );
	}
	*/
	float normalDotL = saturate(dot(normal, L));
	float3 finalDiffuse = diffuse * lightColor * normalDotL;

	//now handle specular term
	float3 V = cameraPos - position; //view vector
	float3 H = normalize( L + V );
	float3 finalSpecular = pow( saturate( dot(normal, H) ), specPower) * lightColor * specular * normalDotL;

	//return ( finalDiffuse + finalSpecular) * atten;
	//return finalSpecular * atten;
	//float3 res = position;
	//float3 res = normal;
	//float3 res = diffuse;
	//float3 res = specular;
	//float3 res = float3(depth, depth, depth);
	//return lightColor;
	//return float3(normalDotL, normalDotL, normalDotL);
	//return diffuse * lightColor * float3(1.0f, 0.0f, 0.0f);
	return finalDiffuse;// * atten;
	//return diffuse;
	//return normalDotL;
}

float4 PS ( float4 screenPos : SV_POSITION ) : SV_Target
{
	float3	position;
	float3	normal;
	float	depth;
	float3	diffuse;
	float3	specular;
	float	specPower;

	GetGBufferAtributes(screenPos.xy, 
						position, normal,
						depth, diffuse,
						specular, specPower);

	float3 color = CalcLightning(position, normal, depth,
							diffuse, specular, specPower);

	return float4(color, 1.0f);
}
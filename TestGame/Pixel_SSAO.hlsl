////////////////////////////////////////////
//Texture resources and sampler states
//////////////////////////////////////////////v
//Texture2D   positionTexture :   register( t0 );
Texture2D	depthTexture	:	register( t0 );
Texture2D	normalTexture	:	register( t1 );

SamplerState pointSampler : register( s0 );

//////////////////////////////////////////////
//Constant buffers
//////////////////////////////////////////////

cbuffer CameraParams	:	register( c0 )
{
    float2 cameraFrustumSize : packoffset(c0);
    float2 cameraParamsPad	 : packoffset(c0.z);
};

cbuffer DepthBufferParams	:	register( c1 )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
{
    float2 depthBufferRecipSize;
    float2 depthBufferOffset;
}; 

cbuffer MatrixBuffer	:	register( c2)
{
	matrix View;
}

float3 ScreenPosToViewPos(float2 screenPos, out float2 screenUV)
{
	//float2 viewSpaceUV = screenPos * depthBufferRecipSize;
	float2 viewSpaceUV = screenPos * float2(1.0 / 1280.0, 1 / 720.0);
	screenUV = viewSpaceUV;

	//[0, 1] to [0, 2]
	viewSpaceUV = viewSpaceUV * float2(2.0f, -2.0f);
	//[0, 2] to [-1, 1]
	viewSpaceUV = viewSpaceUV + float2(-1.0f, 1.0f);
	viewSpaceUV = viewSpaceUV * cameraFrustumSize * 0.5f;

	return float3(viewSpaceUV.x, viewSpaceUV.y, 1.0f) * depthTexture.Sample(pointSampler, screenUV).r;
}

float4 SSAO_PS( float4 screenPos : SV_POSITION ) : SV_TARGET
{
	//get position in view space
	float2 screenUV;
    float3 origin = ScreenPosToViewPos(screenPos.xy, screenUV);

	//get normal in view space
	float4 f4Normal = normalTexture.Sample(pointSampler, screenUV);
	float3 normal = mul(f4Normal, View);

	//Now, reorient basis along origin's normal
	float3 rvec = randomTex.Sample(pointSampler, screenUV * noiseScale).xyz * 2.0f - 1.0f;
	float3 tangent = normalize(rvec - normal * dot(rvec, normal));
	float3 bitangent = cross(normal, tangent);

	float3x3 tbn = float3x3(	tangent,
								bitangent,
								normal );

	float occlusion = 0.0f;
	for (int i = 0; i < sampleKernelSize; i++)
	{
		//get sample position
		float3 sample = mul(sampleKernel[i], tbn);
		sample = sample * radius + origin;

		//get screen uv
		float2 samplePointUV = sample.xy / samplePoint.z;
		samplePointUV = samplePointUV / cameraFrustumSize / 0.5f;
		samplePointUV = samplePointUV + float2(1.0f, -1.0f);
		samplePointUV = samplePointUV * float2(0.5f, -0.5f);

		//get sample depth
		float sampleDepth = depthTexture.Sample(pointSampler, screenUV);

		//accumulate
		float distance = abs(origin.x - sampleDepth) < radius ? 1.0 : 0.0;
		occlusion += (sampleDepth <= sample.z ? 1.0f : 0.0f) * distance;
	}

	occlusion = 1.0 - (occlusion / sampleKernelSize);

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
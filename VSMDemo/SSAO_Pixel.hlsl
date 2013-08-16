//////////////////////////////////////////////
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

cbuffer SSAOParams	:	register( c3 )
{
    //float	fNoOcclusionThreshold;
    //float	fFullOcclusionThreshold;
	//float	fOcclusionRadius;
	//float	fOcclusionPower;
};

//cbuffer SSAOSamplePoints	:	register( c1 )
//{
//	float3 SSAOSamplePoints[8];
//}

/*float3 SSAOSamplePoints [] = 
{
	float3(0.707106,	0.0,		-0.707106),
	float3(0.5,			0.5,		-0.707106),
	float3(0.0,			0.707106,	-0.707106),
	float3(-0.5,		0.5,		-0.707106),
	float3(-0.707106,	0.0,		-0.707106),
	float3(-0.5,		-0.5,		-0.707106),
	float3(0.0,			-0.707106,	-0.707106),
	float3(0.5,			-0.5,		-0.707106)
};*/


float OcclusionFunction(    float fDistance, float fNoOcclusionThreshold,
                            float fFullOcclusionThreshold, float fOcclusionPower)
{
    const float fOcclusionEpsilon = 0.01f;
    
    //no occlusion if distance <= 0
    if (fDistance > fOcclusionEpsilon)
    {
        //calculate occlusion range
        float fOcclusionRange = fNoOcclusionThreshold - fFullOcclusionThreshold;
        
        if (fDistance < fFullOcclusionThreshold)
            return 1.0f;
        else
            return max( 1.0f - pow((fDistance - fFullOcclusionThreshold) / fOcclusionRange,
                                fOcclusionPower), 0.0f); 
    } 
	else
	{
		return 0.0f;
	}
}


float Occlusion(float3 viewPos, float3 SampleDelta,
                float fOcclusionRadius, float fFullOcclusionThreshold,
                float fNoOcclusionThreshold, float fOcclusionPower)
{
    //calculate position of new sample point
    float3 samplePoint = viewPos + fOcclusionRadius * SampleDelta;

    //calculate its UV coords
    float2 samplePointUV = samplePoint.xy / samplePoint.z;
    samplePointUV = samplePointUV / cameraFrustumSize / 0.5f;
    samplePointUV = samplePointUV + float2(1.0f, -1.0f);
    samplePointUV = samplePointUV * float2(0.5f, -0.5f);
    
	float fSampleDepth = depthTexture.Sample(pointSampler, samplePointUV).r;

	if (viewPos.z < fSampleDepth) return 0.0f;

    float fDistance = samplePoint.z - fSampleDepth;
    return OcclusionFunction (  fDistance, fNoOcclusionThreshold,
                              fFullOcclusionThreshold, fOcclusionPower
                               ); 
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

float4 SSAO_PS ( float4 screenPos : SV_POSITION ) : SV_Target
{
	float3 SSAOSamplePoints [] = 
{
	//float3(0.707106,	0.0,		-0.707106),
	//float3(0.5,			0.5,		-0.707106),
	//float3(0.0,			0.707106,	-0.707106),
	//float3(-0.5,		0.5,		-0.707106),
	//float3(-0.707106,	0.0,		-0.707106),
	//float3(-0.5,		-0.5,		-0.707106),
	//float3(0.0,			-0.707106,	-0.707106),
	//float3(0.5,			-0.5,		-0.707106)
	float3(0.5381, 0.1856, -0.4319),
	float3(0.1379, 0.2486, 0.4430),
	float3(0.3317, 0.5679, -0.0057),
	float3(-0.6999, -0.0451, -0.0019),
	float3(0.0689, -0.1598, -0.8547),
	float3(0.0560, 0.0069, -0.1843),
	float3(-0.0146, 0.1402, 0.0762),
	float3(0.0100, -0.1924, -0.0344),
	float3(-0.3577, -0.5301, -0.4358),
	float3(-0.3169, 0.1063, 0.0158),
	float3(0.0103, -0.5869, 0.0046),
	float3(-0.0897, -0.4940, 0.3287),
	float3(0.0352, -0.0631, 0.5460)
};

	//(20.0f, 2.0f, 5.0f, 2.0f);
	float	fNoOcclusionThreshold = 12.0f;
    float	fFullOcclusionThreshold = 1.0f;
	float	fOcclusionRadius = 0.05f;
	float	fMaxRadius = 3.0f;
	float	fOcclusionPower = 2.0f;
    //get view space position and UV coords
	float2 screenUV;
    float3 viewPos = ScreenPosToViewPos(screenPos.xy, screenUV);
   // screenUV = screenPos.xy * depthBufferRecipSize;

    
    //calculate occlusion
    float fAccumBlock = 0.0f;
	float3 SampleDelta;
	int numUsed = 0;
    for (int i = 0; i < 13; i++)
    {
        SampleDelta = SSAOSamplePoints[i];
		float4 SampleDeltaVS = mul(float4(SampleDelta, 0.0f), View);
		SampleDelta = float3(SampleDeltaVS.x, SampleDeltaVS.y, SampleDeltaVS.z);
		SampleDelta = normalize(SampleDelta);
		float3 normal  = normalTexture.Sample(pointSampler, screenUV).rgb;
		float4 normalVS = mul(float4(normal, 0.0f), View);
		normal = float3(normalVS.x, normalVS.y, normalVS.z);
		normal = normalize(normal);
		if (dot(normal, SampleDelta) < 0)
		{
			SampleDelta = -SampleDelta;
			//continue;
		}
		numUsed++;

		float depth = depthTexture.Sample(pointSampler, screenUV).r;
		float scale = min(fMaxRadius, fOcclusionRadius /(1 - depth) );
		//float scale = 1.0f;

        float fBlock = Occlusion(   viewPos,
                                    SampleDelta,
                                    fOcclusionRadius * scale,
                                    fFullOcclusionThreshold,
                                    fNoOcclusionThreshold,
                                    fOcclusionPower ); 
        fAccumBlock += fBlock;
    } 
   // fAccumBlock /= 13;
	fAccumBlock /= numUsed;
	if (fAccumBlock > 0.0f)
	{
		//fAccumBlock = 1.0f;
	}
    
    return 1.0 - fAccumBlock; 
	//return fAccumBlock;
	//return float4(SampleDelta, 1.0f);
	//return float4(screenUV.y, screenUV.y, screenUV.y, 1.0f);
	//return float4(viewPos, 1.0f);
	//return float4(screenPos.x / 1280.0, screenPos.x / 1280, screenPos.x / 1280.0, 1.0f);
}
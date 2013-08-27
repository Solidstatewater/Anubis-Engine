////////////////////////////////////////////
//Constant buffers
//////////////////////////////////////////////
//Texture resources and sampler states
//////////////////////////////////////////////v
//Texture2D   positionTexture :   register( t0 );
//Texture2D	normalTexture	:	register( t1 );
Texture2D	depthTexture	:	register( t0 );

SamplerState anisotropicSampler : register( s0 );

//////////////////////////////////////////////
//Constant buffers
//////////////////////////////////////////////

cbuffer MatrixBuffer	:	register( c0 )
{
	float4x4 WVP;
};

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

cbuffer SSAOParams	:	register( c2 )
{
    float	fNoOcclusionThreshold;
    float	fFullOcclusionThreshold;
	float	fOcclusionRadius;
	float	fOcclusionPower;
};

////////////////////////////////////////
//shaders input structures
////////////////////////////////////////
struct vs_input
{
	float3 pos		: POSITION;
};

float4 SSAO_VS( vs_input input ) : SV_POSITION
{
	return mul(float4(input.pos, 1.0f), WVP);
}

float3 SSAOSamplePoints [] = 
{
	float3(0.707106,	0.0,		-0.707106),
	float3(0.5,			0.5,		-0.707106),
	float3(0.0,			0.707106,	-0.707106),
	float3(-0.5,		0.5,		-0.707106),
	float3(-0.707106,	0.0,		-0.707106),
	float3(-0.5,		-0.5,		-0.707106),
	float3(0.0,			-0.707106,	-0.707106),
	float3(0.5,			-0.5,		-0.707106)
};


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
    else return 0.0f;
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
    samplePointUV = samplePointUV + float2(-1.0f, -1.0f);
    samplePointUV = samplePointUV * float2(0.5f, 0.5f);
    
    //float fSampleDepth = tex2D(depthTexture, samplePointUV).r;
	float fSampleDepth = depthTexture.Sample(anisotropicSampler, samplePointUV).r;
    float fDistance = samplePoint.z - fSampleDepth;
    return OcclusionFunction (  fDistance, fNoOcclusionThreshold,
                                fFullOcclusionThreshold, fOcclusionPower
                                );
}

float3 ScreenPosToViewPos(float3 screenPos, out float2 screenUV)
{
	float2 viewSpaceUV = (float2)screenPos * depthBufferRecipSize;
	screenUV = viewSpaceUV;

	//[0, 1] to [0, 2]
	viewSpaceUV = viewSpaceUV * float2(2.0f, -2.0f);

	//[0, 2] to [-1, 1]
	viewSpaceUV = viewSpaceUV + float2(-1.0f, 1.0f);
	viewSpaceUV = viewSpaceUV * cameraFrustumSize * 0.5f;

	return float3(viewSpaceUV.x, viewSpaceUV.y, 1.0f) * depthTexture.Sample(anisotropicSampler, screenUV).r;
}

float4 SSAO_PS ( float4 screenPos : SV_POSITION ) : SV_Target
{
    //get view space position and UV coords
	/*float2 screenUV;
    float3 viewPos = ScreenPosToViewPos((float3)screenPos, screenUV);
    
    //calculate occlusion
    half fAccumBlock = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        float3 SampleDelta = SSAOSamplePoints[i];
        float fBlock = Occlusion(   viewPos,
                                    SampleDelta,
                                    fOcclusionRadius,
                                    fFullOcclusionThreshold,
                                    fNoOcclusionThreshold,
                                    fOcclusionPower );
        fAccumBlock += fBlock;
    }
    fAccumBlock /= 8;
    
    return 1.0f - fAccumBlock; */
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
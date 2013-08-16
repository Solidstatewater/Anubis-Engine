cbuffer Noise
{
	float noiseVectors[4][4];
};

RWTexture2D<float4> noiseTexture : register( u0 );

[numthreads(4, 4, 1)]
void noise( uint3 DTid : SV_DispatchThreadID )
{
	noiseTexture[DTid.xy] = noiseVectors[DTid.x][DTid.y];
}
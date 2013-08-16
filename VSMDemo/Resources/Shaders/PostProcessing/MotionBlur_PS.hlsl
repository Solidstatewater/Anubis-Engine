////////////////////////////////////////
//structures
struct ps_input
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

//shader resources views
Texture2D sourceTex		: register( t0 );
Texture2D velocityTex	: register( t1 );

SamplerState texSampler : register( s0 );

float4 MotionBlur_PS( ps_input input ) : SV_TARGET
{
	const int maxSamples = 6;
	int numSamples = 0;

	//float2 texCoord = input.texCoord;
	float2 texCoord = input.pos.xy / float2(1280.0f, 720.0f);
	float4 color = sourceTex.Sample(texSampler, texCoord);
	//float4 color = sourceTex.Load(int3(input.pos.xy, 0.0f));
	float4 velocity = velocityTex.Sample(texSampler, texCoord);
	//velocity *= 1000.0f;
	//return velocity;
	//return float4(0.5f, 0.5f, 0.5f, 1.0f);
	//return float4(0, 0, 0, 1.0f);

	texCoord += velocity;
	for (numSamples = 1; numSamples < maxSamples; ++numSamples, texCoord += velocity)
	{
		if (texCoord.x < 0.1 || texCoord.x > 0.9f || texCoord.y < 0.1f || texCoord.y > 0.9f) break;

		float4 nextColor = sourceTex.Sample(texSampler, texCoord);
		color += nextColor;
	}

	color /= numSamples;
	return color; 
}
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
	const int numSamples = 4;

	//float2 texCoord = input.texCoord;
	float2 texCoord = input.pos.xy / float2(1280.0f, 720.0f);
	float4 color = sourceTex.Sample(texSampler, texCoord);
	//float4 color = sourceTex.Load(int3(input.pos.xy, 0.0f));
	float4 velocity = velocityTex.Sample(texSampler, texCoord);
	return color;
	//return velocity;
	//return float4(velocity.x, velocity.y, velocity.x, 1.0f);

	/*texCoord += velocity;
	for (int i = 1; i < numSamples; ++i, texCoord += velocity)
	{
		float4 nextColor = sourceTex.Sample(texSampler, texCoord);
		color += nextColor;
	}

	color /= numSamples;
	return color; */
}
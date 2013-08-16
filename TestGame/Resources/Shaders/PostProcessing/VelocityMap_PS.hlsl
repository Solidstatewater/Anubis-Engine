////////////////////////////////////////
//structures
struct ps_input
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};


////////////////////////////////////
//constant buffers
cbuffer MatrixBuffer : register( b0 )
{
	matrix prevViewProj;
	matrix currViewProj;
}


////////////////////////////////////
//shader resouerce view
Texture2D positionTex : register( t0 );
SamplerState pointSampler : register( s0 );

float4 GenerateVelocityMap_PS( ps_input input ) : SV_TARGET
{
	//calc viewport position
	//float2 currViewportPos = input.pos.xy / input.pos.w;
	//float2 currViewportPos = input.pos.xy / float2(1280.0f, 720.0f);
	//float3 worldPos = positionTex.Sample(pointSampler, currViewportPos).xyz;
	//currViewportPos = float2(currViewportPos.x * 2 - 1, (1 - currViewportPos.y) * 2 - 1);
	float3 worldPos = positionTex.Load(int3(input.pos.xy, 0.0f)).xyz;
	//return float4(currViewportPos.y, currViewportPos.y, currViewportPos.y, 1.0f);

	//calculate world position in the previous frame
	/*float4 prevAfterProjPos = mul(float4(worldPos, 1.0f), prevViewProj);
	//float4 prevViewportPos = mul(float4(worldPos, 1.0f), prevViewProj);
	//prevViewportPos /= prevViewportPos.w;
	float2 prevViewportPos = prevAfterProjPos.xy / float2(1280.0f, 720.0f);
	prevViewportPos = float2(prevViewportPos.x * 2 - 1, (1 - prevViewportPos.y) * 2 - 1);
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
	//return float4(prevViewportPos.y, prevViewportPos.y, prevViewportPos.y, 1.0f);

	//finally compute velocity
	float2 velocity = (currViewportPos - prevViewportPos) / 2.0f;
	//return float4(velocity.x, velocity.y, 0.0f, 1.0f); */

	//calculate previous frame viewport pos
	float4 prevAfterProjPos = mul(float4(worldPos, 1.0f), prevViewProj);
	float2 prevViewportPos = prevAfterProjPos.xy / float2(1280.0f, 720.0f);
	prevViewportPos = float2( (prevViewportPos.x - 0.5f) * 2, (-prevViewportPos.y + 0.5f) * 2);

	//calculate current frame viewport pos
	float4 currAfterProjPos = mul(float4(worldPos, 1.0f), currViewProj);
	float2 currViewportPos = currAfterProjPos.xy / float2(1280.0f, 720.0f);
	currViewportPos = float2( (currViewportPos.x - 0.5f) * 2, (-currViewportPos.y + 0.5f) * 2);

	//calculate velocity
	float2 velocity = (currViewportPos - prevViewportPos) / 2.0f;
	return float4(velocity.x, velocity.y, 0.0f, 1.0f);
}
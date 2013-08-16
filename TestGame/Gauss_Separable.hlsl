//resouces
Texture2D<float4>	InputTex  : register( t0 );
RWTexture2D<float4> OutputTex : register ( u0 );

//group size
#define group_size_x	640
#define	group_size_y	360
#define group_size_z	1

//separable filter kernel coefficients
static const float kernel[7] = {
	0.030078323, 0.1049833664, 0.222250419, 0.285375187, 0.222240419, 0.104983664, 0.030078323
};

groupshared float4 hor_pixels[3+group_size_x+3][3];

//Horizontal blur
[numthreads(group_size_x, 1, 1)]
void CS_Horizontal( uint3 DispatchThreadId : SV_DispatchThreadID , uint3 GroupThreadId : SV_GroupThreadID, uint GI : SV_GroupIndex )
{
	//read the data from the texture so, that we don't load any data from texture aymore
	float4 texel = InputTex.Load(DispatchThreadId);

	hor_pixels[GroupThreadId.x+3][0] = texel * kernel[0];
	hor_pixels[GroupThreadId.x+3][1] = texel * kernel[1];
	hor_pixels[GroupThreadId.x+3][2] = texel * kernel[2];

	//handle edge cases
	if (GI == 0)
	{
		float4 texel0 = InputTex.Load(DispatchThreadId - int3(3,0,0));
		float4 texel1 = InputTex.Load(DispatchThreadId - int3(2,0,0));
		float4 texel2 = InputTex.Load(DispatchThreadId - int3(1,0,0));

		hor_pixels[0][0] = texel0 * kernel[0];
		hor_pixels[1][0] = texel1 * kernel[0];
		hor_pixels[2][0] = texel2 * kernel[0];

		hor_pixels[0][1] = texel0 * kernel[1];
		hor_pixels[1][1] = texel1 * kernel[1];
		hor_pixels[2][1] = texel2 * kernel[1];

		hor_pixels[0][2] = texel0 * kernel[2];
		hor_pixels[1][2] = texel1 * kernel[2];
		hor_pixels[2][2] = texel2 * kernel[2];
	}
	if (GI == (group_size_x - 1))
	{
		float4 texel0 = InputTex.Load(DispatchThreadId + int3(1,0,0));
		float4 texel1 = InputTex.Load(DispatchThreadId + int3(2,0,0));
		float4 texel2 = InputTex.Load(DispatchThreadId + int3(3,0,0));

		hor_pixels[3+group_size_x+0][0] = texel0 * kernel[0];
		hor_pixels[3+group_size_x+1][0] = texel1 * kernel[0];
		hor_pixels[3+group_size_x+2][0] = texel2 * kernel[0];

		hor_pixels[3+group_size_x+0][1] = texel0 * kernel[1];
		hor_pixels[3+group_size_x+1][1] = texel1 * kernel[1];
		hor_pixels[3+group_size_x+2][1] = texel2 * kernel[1];

		hor_pixels[3+group_size_x+0][2] = texel0 * kernel[2];
		hor_pixels[3+group_size_x+1][2] = texel1 * kernel[2];
		hor_pixels[3+group_size_x+2][2] = texel2 * kernel[2];
	}

	//synchronize threads
	GroupMemoryBarrierWithGroupSync();

	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	finalColor += hor_pixels[GI+0][0];
	finalColor += hor_pixels[GI+1][1];
	finalColor += hor_pixels[GI+2][2];
	finalColor += texel * kernel[3];
	finalColor += hor_pixels[GI+4][2];
	finalColor += hor_pixels[GI+5][1];
	finalColor += hor_pixels[GI+6][0];

	//write the result to the output resource
	//OutputTex[DispatchThreadId.xy] = finalColor;
	OutputTex[DispatchThreadId.xy] = float4(1.0f, 0.0f, 0.0f, 1.0f);
}

Texture2D<float4>	VerInputTex : register( t1 );
RWTexture2D<float4> VerOutputTex : register( u1 );

//Vertical blur
groupshared float4 ver_pixels[3+group_size_y+3][3];

[numthreads(1, group_size_y, 1)]
void CS_Vertical( uint3 DispatchThreadId : SV_DispatchThreadID, uint3 GroupThreadId : SV_GroupThreadID, uint GI : SV_GroupIndex )
{
	//read the data from the texture so, that we don'tload any data from texture aymore
	float4 texel = VerInputTex.Load(DispatchThreadId);

	ver_pixels[GroupThreadId.y+3][0] = texel * kernel[0];
	ver_pixels[GroupThreadId.y+3][1] = texel * kernel[1];
	ver_pixels[GroupThreadId.y+3][2] = texel * kernel[2];

	//handle edge cases
	if (GI == 0)
	{
		float4 texel0 = VerInputTex.Load(DispatchThreadId - int3(0,3,0));
		float4 texel1 = VerInputTex.Load(DispatchThreadId - int3(0,2,0));
		float4 texel2 = VerInputTex.Load(DispatchThreadId - int3(0,1,0));

		ver_pixels[0][0] = texel0 * kernel[0];
		ver_pixels[1][0] = texel1 * kernel[0];
		ver_pixels[2][0] = texel2 * kernel[0];

		ver_pixels[0][1] = texel0 * kernel[1];
		ver_pixels[1][1] = texel1 * kernel[1];
		ver_pixels[2][1] = texel2 * kernel[1];

		ver_pixels[0][2] = texel0 * kernel[2];
		ver_pixels[1][2] = texel1 * kernel[2];
		ver_pixels[2][2] = texel2 * kernel[2];
	}
	if (GI == (group_size_y - 1))
	{
		float4 texel0 = VerInputTex.Load(DispatchThreadId + int3(0,1,0));
		float4 texel1 = VerInputTex.Load(DispatchThreadId + int3(0,2,0));
		float4 texel2 = VerInputTex.Load(DispatchThreadId + int3(0,3,0));

		ver_pixels[3+group_size_y+0][0] = texel0 * kernel[0];
		ver_pixels[3+group_size_y+1][0] = texel1 * kernel[0];
		ver_pixels[3+group_size_y+2][0] = texel2 * kernel[0];

		ver_pixels[3+group_size_y+0][1] = texel0 * kernel[1];
		ver_pixels[3+group_size_y+1][1] = texel1 * kernel[1];
		ver_pixels[3+group_size_y+2][1] = texel2 * kernel[1];

		ver_pixels[3+group_size_y+0][2] = texel0 * kernel[2];
		ver_pixels[3+group_size_y+1][2] = texel1 * kernel[2];
		ver_pixels[3+group_size_y+2][2] = texel2 * kernel[2];
	}
	

	//synchronize threads
	GroupMemoryBarrierWithGroupSync();

	//now compose final color!
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		
	finalColor += ver_pixels[GI+0][0];
	finalColor += ver_pixels[GI+1][1];
	finalColor += ver_pixels[GI+2][2];
	finalColor += texel * kernel[3];
	finalColor += ver_pixels[GI+4][2];
	finalColor += ver_pixels[GI+5][1];
	finalColor += ver_pixels[GI+6][0];

	//write the result to the output resource
	//VerOutputTex[DispatchThreadId.xy] = finalColor;
	VerOutputTex[DispatchThreadId.xy] = float4(1.0f, 0.0f, 0.0f, 1.0f);
}
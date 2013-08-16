//Resources
Texture2D<float4>	InputTex	:	register( t0 );
RWTexture2D<float4>	OutputTex	:	register( u0 );

//group size
#define size_x	32
#define	size_y	32

//filter kernel coefficients
static const float kernel[7][7] = {
	0.000904706, 0.003157733, 0.00668492,  0.008583607, 0.00668492,  0.003157733, 0.000904706,
	0.003157733, 0.01102157,  0.023332663, 0.029959733, 0.023332663, 0.01102157,  0.003157733,
	0.00668492,  0.023332663, 0.049395249, 0.063424755, 0.049395249, 0.023332663, 0.00668492,
	0.008583607, 0.029959733, 0.063424755, 0.081438997, 0.063424755, 0.029959733, 0.008583607,
	0.00668492,  0.023332663, 0.049395249, 0.063424755, 0.049395249, 0.023332663, 0.00668492,
	0.003157733, 0.01102157,  0.023332663, 0.029959733, 0.023332663, 0.01102157,  0.003157733,
	0.000904706, 0.003157733, 0.00668492,  0.008583607, 0.00668492,  0.003157733, 0.000904706
};

//one thread for each texel
[numthreads(size_x, size_y, 1)]
void CS(uint3 GroupId : SV_GroupID, uint3 DispatchThreadId : SV_DispatchThreadID, uint3 GroupThreadId : SV_GroupThreadID,
		uint GroupIndex : SV_GroupIndex )
{
	float4 texel = InputTex.Load(DispatchThreadId);

	//get coordinates of top left corner
	int3 textureCoords = DispatchThreadId - int3(3, 3, 0);

	//initialize final color
	float4 finalColor = (float4)0.0f;

	//loop through filter kernel getting composing color
	for (int  i = 0; i < 7; i++)
		for (int  j = 0; j < 7; j++)
			finalColor += InputTex.Load(textureCoords + int3(i, j, 0) ) * kernel[i][j];

	//linear interpolation between actual texel color and blurred one
	finalColor = finalColor;
	finalColor.w = 1.0f;

	//assign new color value to the output image
	OutputTex[DispatchThreadId.xy] = finalColor;
}
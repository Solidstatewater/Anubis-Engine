cbuffer MatrixBuffer
{
	matrix OrthoWVP;
};

float4 Shadows_VS( float3 pos : POSITION ) : SV_POSITION
{
	return mul(float4(pos, 1.0f), OrthoWVP);
}
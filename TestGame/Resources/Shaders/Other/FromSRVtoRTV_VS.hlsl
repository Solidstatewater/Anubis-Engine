cbuffer MatrixBuffer : register( b0 )
{
	matrix WVP;
};

float4 SRVtoRTV_VS( float3 pos : POSITION ) : SV_POSITION
{
	return mul(float4(pos, 1.0f), WVP);
}
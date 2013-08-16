Texture2D sourceTex : register( t0 );

float4 SRVtoRTV_PS( float4 pos : SV_POSITION) : SV_TARGET
{
	float4 color = sourceTex.Load(int3(pos.xy, 0.0f));
	return color;
}
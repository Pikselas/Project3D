struct vtx
{
	float3 col : COLOR;
	float4 pos : SV_POSITION;
};

vtx main(float2 pos : POSITION, float3 col : COLOR)
{
	vtx Out;
	Out.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	Out.col = col;
	return Out;
}
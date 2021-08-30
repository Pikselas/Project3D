struct vtx
{
	float3 col : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer cbf
{
	row_major matrix transform;
};

vtx main(float2 pos : POSITION, float3 col : COLOR)
{
	vtx Out;
	Out.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f),transform);
	Out.col = col;
	return Out;
}
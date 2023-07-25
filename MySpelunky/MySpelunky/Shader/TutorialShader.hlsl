
// Symentic Name
// SV : SystemValue

struct VertexInput
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VertexOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput result;
	result.pos = input.pos;
	result.color = input.color;

	return result;
}

float4 PS(VertexOutput input) : SV_TARGET
{
	return input.color;
}
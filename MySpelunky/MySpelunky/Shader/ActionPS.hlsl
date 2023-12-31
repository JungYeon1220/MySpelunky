Texture2D resource : register(t0);
SamplerState samp : register(s0);

cbuffer ReverseBuffer : register(b0)
{
	int isReverse;
	int padding2[3];
}

cbuffer ActionBuffer : register(b1)
{
	float2 startPos;
	float2 size;
	float2 imageSize;
	int padding[2];
}

struct PixelInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : UV;
};

float4 PS(PixelInput input) : SV_TARGET
{
	float2 resultUV;
// input.x ... 0 ~ 1
// startPos (0,520)
// ImageSize(1200, 1040)
// 
if (isReverse == 1)
input.uv.x = 1 - input.uv.x;

resultUV.x = (startPos.x) / (imageSize.x) + input.uv.x * (size.x / imageSize.x);
resultUV.y = (startPos.y) / (imageSize.y) + input.uv.y * (size.y / imageSize.y);

float4 color = resource.Sample(samp, resultUV);

return color;
}
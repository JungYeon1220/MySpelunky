
Texture2D resource : register(t0);
SamplerState samp : register(s0);

cbuffer FrameBuffer : register(b0)
{
    float2 maxFrame;
    float2 curFrame;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
};

float4 PS(PixelInput input) : SV_TARGET
{
	float2 resultUV;

// curFrame = (0,1)
// maxFrame = (10,8)
// input 0,0
// resultUVx : 0
// resultUVy : 1/8
// input 1,1
// resultUVx : 1/10
// reusltUVy : 2/8
    resultUV.x = input.uv.x / maxFrame.x + curFrame.x / maxFrame.x;
    resultUV.y = input.uv.y / maxFrame.y + curFrame.y / maxFrame.y;

    float4 color = resource.Sample(samp, resultUV);

    return color;
}
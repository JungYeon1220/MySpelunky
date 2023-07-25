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
};

cbuffer FilterBuffer : register(b2)
{
	int selected;
	int value1;
	int value2;
	int value3;

	float2 imageSize2;
	float2 radialCenter;
}

struct PixelInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : UV;
};

float4 Mosaic(float2 uv)
{
	// 0 ~ 1 * 10 .. 0.5 => 5 , 0.55 => 5
	float x = floor(uv.x * value1); // floor : 소수뒷자리 날리는 함수
	float y = floor(uv.y * value1);

	float2 temp;
	temp.x = x / value1; // 0.5
	temp.y = y / value1;

	return resource.Sample(samp, temp);
}

float4 Blur(float2 uv)
{
	float4 result = 0;

	if (value2 == 0)
		return resource.Sample(samp, uv);

	for (int i = 0; i < value2; i++)
	{
		float divX = (i + 1) / imageSize2.x;
		float divY = (i + 1) / imageSize2.y;

		result += resource.Sample(samp, float2(uv.x + divX, uv.y));
		result += resource.Sample(samp, float2(uv.x - divX, uv.y));
		result += resource.Sample(samp, float2(uv.x, uv.y + divY));
		result += resource.Sample(samp, float2(uv.x, uv.y - divY));
	}

	result /= 4 * value2;

	return result;
}

float4 OctaBlur(float2 uv)
{
	float4 result = 0;

	if (value3 == 0)
		return resource.Sample(samp, uv);

	for (int i = 0; i < value3; i++)
	{
		float divX = (i + 1) / imageSize2.x;
		float divY = (i + 1) / imageSize2.y;

		result += resource.Sample(samp, float2(uv.x + divX, uv.y));
		result += resource.Sample(samp, float2(uv.x - divX, uv.y));
		result += resource.Sample(samp, float2(uv.x, uv.y + divY));
		result += resource.Sample(samp, float2(uv.x, uv.y - divY));

		result += resource.Sample(samp, float2(uv.x + divX, uv.y - divY));
		result += resource.Sample(samp, float2(uv.x + divX, uv.y + divY));
		result += resource.Sample(samp, float2(uv.x - divX, uv.y - divY));
		result += resource.Sample(samp, float2(uv.x - divX, uv.y + divY));
	}

	result /= 8 * value3;

	return result;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float2 resultUV;

	if (isReverse == 1)
		input.uv.x = 1 - input.uv.x;

	resultUV.x = (startPos.x) / (imageSize.x) + input.uv.x * (size.x / imageSize.x);
	resultUV.y = (startPos.y) / (imageSize.y) + input.uv.y * (size.y / imageSize.y);

	if (selected == 0)
		return resource.Sample(samp, resultUV);
	else if (selected == 1)
		return Mosaic(resultUV);
	else if (selected == 2)
		return Blur(resultUV);
	else if (selected == 3)
		return OctaBlur(resultUV);

	return resource.Sample(samp, input.uv);
}
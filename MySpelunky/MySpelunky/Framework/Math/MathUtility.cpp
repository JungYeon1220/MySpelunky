#include "framework.h"
#include "MathUtility.h"

int MathUtility::RandomInt(int min, int max)
{
    // 50 ~ 100

    return rand() % (max - min) + min;
}

float MathUtility::RandomFloat(float min, float max)
{
    // 5.0f ~ 6.0f;
    float temp = rand() / (float)RAND_MAX;

    return (max - min) * temp + min;
}

Vector2 MathUtility::GetGridPosition(Vector2 pos)
{
	pos *= 0.01f;
	pos.x = round(pos.x);
	pos.y = round(pos.y);
	pos *= 100.0f;

	return pos;
}

Vector2 MathUtility::GetGridIndex(Vector2 pos)
{
	pos *= 0.01f;
	pos.x = round(pos.x);
	pos.y = round(pos.y);

	return Vector2((int)pos.x, 35 - (int)pos.y);
}

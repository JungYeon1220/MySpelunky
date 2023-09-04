#pragma once
class MathUtility
{
public:
	static int RandomInt(int min, int max);

	static float RandomFloat(float min, float max);

	static Vector2 GetGridPosition(Vector2 pos);
	static Vector2 GetGridIndex(Vector2 pos);

};

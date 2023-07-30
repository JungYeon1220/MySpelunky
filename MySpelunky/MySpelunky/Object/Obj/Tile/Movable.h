#pragma once
class Movable : public Tile
{
public:
	Movable();
	Movable(Vector2 pos);
	~Movable();

	virtual void Update() override;
	virtual void Render() override;

private:
	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxFalling = 700.0f;
};
#pragma once
class Movable : public Tile
{
public:
	Movable();
	Movable(Vector2 pos);
	virtual ~Movable();

	virtual void Update() override;
	virtual void Render() override;

	shared_ptr<RectCollider> GetMovableCollider() { return _movableCol; }

	bool& IsFalling() { return _isFalling; }
	bool& CanGrab() { return _canGrab; }
private:
	shared_ptr<RectCollider> _movableCol;
	bool _canGrab = false;

	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxFalling = 700.0f;
};
#pragma once
class Movable : public Tile
{
public:
	Movable();
	Movable(Vector2 pos);
	virtual ~Movable();

	virtual void Update() override;
	virtual void Render() override;

	virtual void InteractPlayer(shared_ptr<class Player> player);
	virtual void Destroy() { _isActive = false; }
	shared_ptr<RectCollider> GetMovableCollider() { return _movableCol; }

	bool& IsFalling() { return _isFalling; }
private:
	shared_ptr<RectCollider> _movableCol;

	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxFalling = 700.0f;
};
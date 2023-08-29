#pragma once
class Item
{
public:
	Item();
	virtual ~Item();

	virtual void Update();
	virtual void Render();

	virtual shared_ptr<RectCollider> GetCollider() { return _col;}
	float& GetJumpPower() { return _jumpPower; }
	Vector2 GetSize() { return _size; }

	bool& IsFalling() { return _isFalling; }
	bool& IsActive() { return _isActive; }

	void InteractPlayer(shared_ptr<class Player> player);

protected:
	shared_ptr<Transform> _transform;
	shared_ptr<RectCollider> _col;

	Vector2 _size;

	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxFalling = 700.0f;

	bool _isActive = false;

	string _name = "";
};

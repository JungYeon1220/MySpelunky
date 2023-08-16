#pragma once
class Monster
{
public:
	Monster();
	virtual ~Monster();

	virtual void Update();
	virtual void Render();

	virtual void Left();
	virtual void Right();
	virtual void Reverse() {}

	virtual void Attack(shared_ptr<class Player> player) {}
	virtual void EndAttack() {}

	void TakeDamage(int value);

	shared_ptr<RectCollider> GetCollider() { return _col; }
	shared_ptr<RectCollider> GetRangeCol() { return _rangeCol; }
	Vector2 GetSize() { return _size; }
	bool& IsDead() { return _isDead; }
	float& GetSpeed() { return _curSpeed; }
	bool& IsFalling() { return _isFalling; }
	bool& InRange() { return _inRange; }

protected:
	shared_ptr<Transform> _transform;
	shared_ptr<RectCollider> _col;
	shared_ptr<RectCollider> _rangeCol;
	shared_ptr<Sprite_Frame> _sprite;
	vector<shared_ptr<Action>> _actions;
	Vector2 _size;
	bool _isLeft = false;

	bool _isDead = false;

	bool _inRange = false;

	bool _isAttack = false;

	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxFalling = 700.0f;

	float _curSpeed = 0.0f;
	float _moveSpeed;
	Vector2 _dir = RIGHT_VECTOR;

	int _hp;
};


#pragma once
class Monster
{
public:
	Monster();
	~Monster();

	enum State
	{
		IDLE,
		JUMP
	};

	void Update();
	void Render();

	void Jump();
	void SetTarget(shared_ptr<class Player> player);
	void SetAction(State state);

	shared_ptr<RectCollider> GetCollider() { return _col; }
	shared_ptr<RectCollider> GetRangeCol() { return _rangeCol; }
	float& GetSpeed() { return _speed; }
	bool& IsFalling() { return _isFalling; }
	bool& IsJumping() { return _isJumping; }
	bool& CanJump() { return _canJump; }
	bool& InRange() { return _inRange; }

private:
	void CreateAction();
	shared_ptr<Transform> _transform;
	shared_ptr<RectCollider> _col;
	shared_ptr<RectCollider> _rangeCol;
	shared_ptr<Sprite_Frame> _sprite;
	vector<shared_ptr<Action>> _actions;

	bool _inRange = false;

	bool _isJumping = false;
	bool _canJump = true;
	float _curJumpTime = 0.0f;
	float _jumpCoolTime = 2.0f;

	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxFalling = 700.0f;

	float _speed = 0.0f;
	Vector2 _dir = RIGHT_VECTOR;

	State _curState = State::IDLE;
	State _oldState = State::IDLE;

};


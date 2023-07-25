#pragma once
class Player
{
public:
	enum State
	{
		IDLE,
		RUN,
		DEAD,
		LAY_DOWN,
		CRAWL,
		STUN,
		JUMP,
		ATTACK,
		CLIMB_RADDER,
		LEDGE_GRAB,
		LOOK_UP
	};

	Player();
	~Player();

	void Input();
	void Jump();
	void Attack();
	void ClimbRadder();
	void GrabLedge();

	void Update();
	void Render();
	void PostRender();

	void SetAction(State state);
	void SetIdle()
	{
		if (_isAttack == true)
			return;
		SetAction(State::IDLE);
	}
	void EndAttack();

	shared_ptr<RectCollider> GetCollider() { return _col; }
	shared_ptr<CircleCollider> GetFeetCollider() { return _feetCol; }
	shared_ptr<CircleCollider> GetHeadCollider() { return _headCol; }
	shared_ptr<CircleCollider> GetGrabCollider() { return _grabCol; }
	State GetState() { return _curState; }
	float& GetJumpPower() { return _jumpPower; }
	Vector2 GetSize() { return _playerSize; }

	bool& IsFalling() { return _isFalling; }
	bool& isClimb() { return _isClimb; }
	bool& CanClimb() { return _canClimb; }
	bool& isOnOneWay() { return _onOneWay; }
	bool& isGrab() { return _isGrabLedge; }

private:
	void CreateAction();
	State _curState = State::IDLE;
	State _oldState = State::IDLE;

	Vector2 _playerSize = Vector2(60.0f, 80.0f);

	shared_ptr<RectCollider> _col;
	shared_ptr<RectCollider> _crouchCol;
	shared_ptr<CircleCollider> _feetCol;
	shared_ptr<CircleCollider> _headCol;
	shared_ptr<CircleCollider> _grabCol;
	shared_ptr<Transform> _transform;
	shared_ptr<Transform> _upTransform;
	shared_ptr<Transform> _downTransform;
	vector<shared_ptr<Action>> _actions;
	shared_ptr<Sprite_Frame> _sprite;

	shared_ptr<class Whip> _whip;

	bool _isLeft = false;

	float _lookTime = 0.0f;

	float _speed = 300.0f;
	bool _isLaying = false;

	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxFalling = 700.0f;

	bool _isAttack = false;
	bool _isClimb = false;

	bool _canClimb = true;
	float _climbTime = 0.0f;

	bool _onOneWay = false;

	bool _isGrabLedge = false;
};


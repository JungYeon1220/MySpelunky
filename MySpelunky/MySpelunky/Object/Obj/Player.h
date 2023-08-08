#pragma once
class Player
{
public:
	enum State
	{
		IDLE,
		RUN,
		LAY_DOWN,
		CRAWL,
		STUN_AIR,
		JUMP,
		ATTACK,
		CLIMB_RADDER,
		LEDGE_GRAB,
		LOOK_UP,
		STUN_GROUND,
		PUSH
	};

	Player();
	~Player();

	void Input();
	void Jump();
	void SetTarget();
	void ClimbRadder();
	void GrabLedge();
	void Push();

	void Update();
	void Render();
	void PostRender();

	void SetAction(State state);
	void SetIdle();
	void EndAttack();
	void TakeDamage(int value);
	void Dead();

	shared_ptr<RectCollider> GetCollider() { return _col; }
	shared_ptr<CircleCollider> GetFeetCollider() { return _feetCol; }
	shared_ptr<CircleCollider> GetHeadCollider() { return _headCol; }
	shared_ptr<CircleCollider> GetGrabCollider() { return _grabCol; }
	shared_ptr<RectCollider> GetHitCollider();
	State GetState() { return _curState; }
	float& GetJumpPower() { return _jumpPower; }
	Vector2 GetSize() { return _playerSize; }
	int GetHp() { return _hp; }

	bool& IsFalling() { return _isFalling; }
	bool& IsClimb() { return _isClimb; }
	bool& CanClimb() { return _canClimb; }
	bool& IsOnOneWay() { return _onOneWay; }
	bool& IsGrab() { return _isGrabLedge; }
	bool& IsStun() {return _isStun;}
	bool& IsDead() {return _isDead;}
	bool& IsPush() { return _isPush; }

private:
	void CreateAction();
	State _curState = State::IDLE;
	State _oldState = State::IDLE;

	Vector2 _playerSize = Vector2(60.0f, 80.0f);

	shared_ptr<RectCollider> _col;
	shared_ptr<RectCollider> _layDownCol;
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
	bool _isDead = false;
	bool _isStun = false;
	bool _isPush = false;

	float _curStunTime = 0.0f;
	float _stunCoolTime = 0.5f;

	bool _canClimb = true;
	float _climbTime = 0.0f;

	bool _onOneWay = false;

	bool _isGrabLedge = false;

	int _hp = 3;
	bool _isDamaged = false;
	float _curDamagedTime = 0.0f;
	float _damagedCoolTime = 2.0f;
};


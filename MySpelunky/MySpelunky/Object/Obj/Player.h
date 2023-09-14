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
		PUSH,
		THROW,
		CLIMB_ROPE
	};

	Player();
	~Player();

	void Input();
	void Jump();
	void Attack();
	void ThrowBomb();
	void ClimbLadder();
	void ClimbRope();
	void GrabLedge();
	void ThrowItem();
	void DropItem();
	bool HoldItem(shared_ptr<Item> item);

	void Update();
	void Render();
	void PostRender();

	void SetAction(State state);
	void SetIdle();
	void EndAttack();
	void EndThrow();
	void TakeDamage(int value);

	void KnockBack(Vector2 pos, float value);
	void StepOn(shared_ptr<Monster> monster);
	void Dead();

	shared_ptr<RectCollider> GetCollider() { return _col; }
	shared_ptr<CircleCollider> GetFeetCollider() { return _feetCol; }
	shared_ptr<CircleCollider> GetHeadCollider() { return _headCol; }
	shared_ptr<CircleCollider> GetGrabCollider() { return _grabCol; }
	shared_ptr<RectCollider> GetHitCollider();

	shared_ptr<class Whip> GetWhip() { return _whip; }
	State GetState() { return _curState; }
	Vector2 GetSize() { return _playerSize; }
	float& GetJumpPower() { return _jumpPower; }
	float& GetJumpMax() { return _maxJump; }
	float& GetSpeed() { return _curSpeed; }
	int GetHp() { return _hp; }

	bool& IsFalling() { return _isFalling; }
	bool& IsClimb() { return _isClimb; }
	bool& IsRope() { return _isRope; }
	bool& CanClimb() { return _canClimb; }
	bool& IsOnOneWay() { return _onOneWay; }
	bool& IsGrab() { return _isGrabLedge; }
	bool& IsStun() {return _isStun;}
	bool& IsDead() {return _isDead;}
	bool& IsPush() { return _isPush; }

	bool _hasGloves = false;
private:
	void CreateAction();
	State _curState = State::IDLE;
	State _oldState = State::IDLE;

	Vector2 _playerSize = Vector2(50.0f, 80.0f);

	shared_ptr<RectCollider> _col;
	shared_ptr<RectCollider> _layDownCol;
	shared_ptr<CircleCollider> _feetCol;
	shared_ptr<CircleCollider> _headCol;
	shared_ptr<CircleCollider> _grabCol;
	shared_ptr<CircleCollider> _handCol;

	shared_ptr<Transform> _transform;
	shared_ptr<Transform> _upTransform;
	shared_ptr<Transform> _downTransform;

	vector<shared_ptr<Action>> _actions;
	shared_ptr<Sprite_Frame> _sprite;
	shared_ptr<PixelShader> _playerPS;
	shared_ptr<DamagedBuffer> _damagedBuffer;

	shared_ptr<class Whip> _whip;

	weak_ptr<Item> _handItem;

	bool _isLeft = false;

	float _lookTime = 0.0f;

	float _curSpeed = 0.0f;
	float _moveSpeed = 300.0f;
	bool _isLaying = false;

	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxJump = 1200.0f;
	float _maxFalling = 700.0f;

	bool _isAttack = false;
	bool _isThrow = false;
	bool _isClimb = false;
	bool _isRope = false;
	bool _isDead = false;
	bool _isStun = false;
	bool _isPush = false;

	float _curStunTime = 0.0f;
	float _stunCoolTime = 3.0f;

	bool _canClimb = true;
	float _climbTime = 0.0f;

	bool _onOneWay = false;

	bool _isGrabLedge = false;

	int _hp = 300;
	bool _isDamaged = false;
	float _curDamagedTime = 0.0f;
	float _damagedCoolTime = 2.0f;

	int _bombCount = 100;
	int _ropeCount = 3;
};


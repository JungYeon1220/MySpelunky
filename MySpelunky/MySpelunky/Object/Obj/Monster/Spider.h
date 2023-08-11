#pragma once
class Spider : public Monster
{
public:
	Spider();
	virtual ~Spider();

	enum State
	{
		IDLE,
		JUMP
	};

	virtual void Update();
	virtual void Render();

	void SetAction(State state);

	void Jump();
	void SetTarget(shared_ptr<class Player> player);

	bool& IsJumping() { return _isJumping; }
	bool& CanJump() { return _canJump; }

private:
	void CreateAction();

	bool _isJumping = false;
	bool _canJump = true;
	float _curJumpTime = 0.0f;
	float _jumpCoolTime = 2.0f;

	State _curState = State::IDLE;
	State _oldState = State::IDLE;
};


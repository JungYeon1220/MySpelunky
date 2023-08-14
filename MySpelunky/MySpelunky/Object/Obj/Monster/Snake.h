#pragma once
class Snake : public Monster
{
public:
	Snake();
	virtual ~Snake();

	enum State
	{
		IDLE,
		MOVE,
		ATTACK
	};

	virtual void Update();
	virtual void Render();

	virtual void Left();
	virtual void Right();
	virtual void Reverse();

	void Move();
	virtual void SetTarget(shared_ptr<class Player> player);
	virtual void EndAttack();

	bool& IsMoving() { return _isMoving; }

	void SetAction(State state);
private:
	void CreateAction();

	bool _isMoving = false;
	float _duration = 0.0f;
	float _maxDuration;

	float _attackTime = 0.0f;

	State _curState = State::IDLE;
	State _oldState = State::IDLE;

};


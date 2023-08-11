#pragma once
class Snake : public Monster
{
public:
	Snake();
	virtual ~Snake();

	enum State
	{
		IDLE,
		MOVE
	};

	virtual void Update();
	virtual void Render();

	virtual void Left();
	virtual void Right();
	virtual void Reverse();

	void SetAction(State state);
private:
	void CreateAction();

	bool _isMoving = false;
	float _duration = 0.0f;
	float _maxDuration;

	State _curState = State::IDLE;
	State _oldState = State::IDLE;

};


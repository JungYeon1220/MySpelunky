#pragma once
class Snake : public Monster
{
public:
	Snake();
	Snake(Vector2 pos);
	virtual ~Snake();

	enum State
	{
		IDLE,
		MOVE,
		ATTACK
	};

	virtual void Update();
	virtual void Render();

	virtual bool TileInteract(shared_ptr<Tile> tile);
	virtual void Land(bool check);

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

	State _curState = State::IDLE;
	State _oldState = State::IDLE;

};


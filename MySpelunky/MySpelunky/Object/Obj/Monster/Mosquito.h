#pragma once
class Mosquito : public Monster
{
public:
	Mosquito();
	Mosquito(Vector2 pos);
	virtual ~Mosquito();

	enum State
	{
		IDLE,
		ATTACK
	};

	virtual void Update();
	virtual void Render();

	virtual bool TileInteract(shared_ptr<Tile> tile);

	void Move();

	virtual void SetTarget(shared_ptr<class Player> player);

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


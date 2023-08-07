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
	void SetAction(State state);

	shared_ptr<RectCollider> GetCollider() { return _col; }
	bool& IsFalling() { return _isFalling; }


private:
	void CreateAction();
	shared_ptr<Transform> _transform;
	shared_ptr<RectCollider> _col;
	shared_ptr<Sprite_Frame> _sprite;
	vector<shared_ptr<Action>> _actions;

	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxFalling = 700.0f;

	State _curState = State::IDLE;
	State _oldState = State::IDLE;

};


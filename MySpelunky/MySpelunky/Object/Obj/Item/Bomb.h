#pragma once
class Bomb : public Item
{
public:
	enum State
	{
		IDLE,
		IMMINENT1,
		INNINENT2
	};

	Bomb();
	virtual ~Bomb();

	virtual void Update();
	virtual void Render();

	bool DestroyTile(shared_ptr<Tile> tile);

	float& GetRotation() { return _rotation; }
	shared_ptr<CircleCollider> GetRangeCol() { return _range; }
	bool _boom = false;
private:
	void SetAction(State state);
	void CreateAction();
	vector<shared_ptr<Action>> _actions;

	shared_ptr<CircleCollider> _range;
	shared_ptr<Transform> _offsetTrans;

	float _rotation = 0.0f;
	float _time = 0.0f;

	State _curState = State::IDLE;
	State _oldState = State::IDLE;
};


#pragma once
class Whip
{
public:
	Whip();
	~Whip();

	void SetTarget();
	void End();

	void Update();
	void Render();

	void SetLeft();
	void SetRight();

	shared_ptr<Transform> GetTransform() { return _transform; }

private:
	shared_ptr<Transform> _transform;
	shared_ptr<Sprite_Frame> _sprite;
	shared_ptr<Action> _action;

	Vector2 _frontPos = Vector2(60.0f, -15.0f);
	Vector2 _backPos = Vector2(-55.0f, 40.0f);

	shared_ptr<RectCollider> _col;

	bool _canStand = false;
	bool _isLeft = false;
};


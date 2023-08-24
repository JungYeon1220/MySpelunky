#pragma once
class Item
{
public:
	Item();
	~Item();

	void Update();
	void Render();

	shared_ptr<RectCollider> GetCollider() { return _col;}
	bool& IsFalling() { return _isFalling; }

	void InteractPlayer(shared_ptr<class Player> player);

private:
	shared_ptr<Transform> _transform;
	shared_ptr<RectCollider> _col;

	bool _isFalling = false;
	float _jumpPower = 0.0f;
	float _maxFalling = 700.0f;

	bool _isActive = true;

	string _name = "";
};

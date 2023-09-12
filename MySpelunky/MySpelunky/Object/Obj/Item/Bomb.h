#pragma once
class Bomb : public Item
{
public:
	Bomb();
	virtual ~Bomb();

	virtual void Update();
	virtual void Rander();

	void DestroyTile(shared_ptr<Tile> tile);

	float& GetRotation() { return _rotation; }
private:
	shared_ptr<CircleCollider> _range;
	shared_ptr<Transform> _offsetTrans;

	bool _boom = false;

	float _rotation = 0.0f;
	float _time = 0.0f;
};


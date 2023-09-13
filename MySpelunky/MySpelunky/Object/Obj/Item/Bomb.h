#pragma once
class Bomb : public Item
{
public:
	Bomb();
	virtual ~Bomb();

	virtual void Update();
	virtual void Rander();

	bool DestroyTile(shared_ptr<Tile> tile);

	float& GetRotation() { return _rotation; }
	shared_ptr<CircleCollider> GetRangeCol() { return _range; }
	bool _boom = false;
private:
	shared_ptr<CircleCollider> _range;
	shared_ptr<Transform> _offsetTrans;

	float _rotation = 0.0f;
	float _time = 0.0f;
};


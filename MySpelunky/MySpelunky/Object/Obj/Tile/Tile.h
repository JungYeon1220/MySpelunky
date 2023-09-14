#pragma once
class Tile
{
public:
	enum Type
	{
		EMPTY,
		NORMAL,
		LADDER,
		ONE_WAY,
		UNBREAKABLE,
		SPIKE,
		SKELETON,
		MOVABLE,
		WOODEN
	};
	Tile();
	Tile(Vector2 pos);
	virtual ~Tile();

	void SetType(Type value);
	void SetPosition(Vector2 pos) { _pos = pos; }
	virtual bool Block(shared_ptr<Collider> col);
	virtual void InteractPlayer(shared_ptr<class Player> player);
	virtual void Destroy() {}

	virtual void Update();
	virtual void Render();

	virtual void PebbleUp() {}
	virtual void PebbleDown() {}
	virtual void PebbleRight() {}
	virtual void PebbleLeft() {}

	shared_ptr<RectCollider> GetCollider() { return _col; }
	Type GetType() { return _type; }
	const string& GetName() { return _name; }
	Vector2& GetIndex() { return _index; }
	bool& CanGrab() { return _canGrab; }
	bool& LedgeRight() { return _cliffR; }
	bool& LedgeLeft() { return _cliffL; }
	bool& IsActive() { return _isActive; }

protected:
	string _name;

	shared_ptr<RectCollider> _col;
	shared_ptr<Transform> _transform;

	Vector2 _pos = Vector2(0.0f, 0.0f);
	Vector2 _selected = Vector2(0, 0);
	Type _type = EMPTY;

	bool _isActive = true;

	bool _canGrab = false;

	bool _cliffR = false;
	bool _cliffL = false;

	Vector2 _index;
};


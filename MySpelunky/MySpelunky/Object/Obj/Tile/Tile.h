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

	virtual void Update();
	virtual void Render();

	shared_ptr<RectCollider> GetCollider() { return _col; }
	Type GetType() { return _type; }

protected:
	shared_ptr<RectCollider> _col;
	shared_ptr<Transform> _transform;
	shared_ptr<Sprite_Frame> _sprite;

	Vector2 _pos = Vector2(0.0f, 0.0f);
	Vector2 _selected = Vector2(0, 0);
	Type _type = EMPTY;

	bool _isActive = true;

};


#pragma once
class Monster
{
public:
	Monster();
	~Monster();

	void Update();
	void Render();

private:
	shared_ptr<Transform> _transform;
	shared_ptr<RectCollider> _col;
	shared_ptr<Sprite_Frame> _sprite;



};


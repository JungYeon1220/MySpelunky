#include "framework.h"
#include "Movable.h"

Movable::Movable()
	:Tile()
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(7, 0));
	_type = Tile::Type::MOVABLE;
}

Movable::Movable(Vector2 pos)
	:Tile(pos)
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(7, 0));
	_type = Tile::Type::MOVABLE;
}

Movable::~Movable()
{
}

void Movable::Update()
{
	if (_isFalling == true)
		_jumpPower -= GRAVITY * 8;
	else
		_jumpPower = 0.0f;

	if (_jumpPower < -_maxFalling)
		_jumpPower = -_maxFalling;

	_col->GetTransform()->AddVector2(Vector2(0.0f, _jumpPower * DELTA_TIME));
}

void Movable::Render()
{
}

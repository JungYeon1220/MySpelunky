#include "framework.h"
#include "OneWay.h"

OneWay::OneWay()
	:Tile()
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floormisc.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(1, 1));
	_type = Tile::Type::ONE_WAY;
}

OneWay::OneWay(Vector2 pos)
	:Tile(pos)
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floormisc.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(1, 1));
	_type = Tile::Type::ONE_WAY;
}

OneWay::~OneWay()
{
}

bool OneWay::Block(shared_ptr<Collider> col)
{
	if (_canStand == false)
		return false;
	Vector2 colPos = col->GetWorldPos();
	Vector2 pos = _col->GetWorldPos();
	Vector2 halfSize = Vector2(50.0f, 50.0f);


	if ((colPos.x < pos.x + halfSize.x && colPos.x > pos.x - halfSize.x)
		&& (colPos.y > pos.y + halfSize.y))
	{
		return _col->Block(col);
	}

	return false;
}

void OneWay::Update()
{
	Tile::Update();
}

void OneWay::Render()
{
	Tile::Render();
}

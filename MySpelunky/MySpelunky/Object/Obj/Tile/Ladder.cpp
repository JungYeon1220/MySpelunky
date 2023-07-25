#include "framework.h"
#include "Ladder.h"

Ladder::Ladder()
	:Tile()
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(4, 0));
	_type = Tile::Type::LADDER;
}

Ladder::Ladder(Vector2 pos)
	:Tile(pos)
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(4, 0));
	_type = Tile::Type::LADDER;
}

Ladder::~Ladder()
{
}

bool Ladder::Block(shared_ptr<Collider> col)
{
	if (_isOneWay == false)
		return false;

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

void Ladder::Update()
{
	Tile::Update();
}

void Ladder::Render()
{
	Tile::Render();
}

void Ladder::SetTop()
{
	_sprite->SetCurClip(Vector2(4, 0));
	_isOneWay = false;
}

void Ladder::SetMiddle()
{
	_sprite->SetCurClip(Vector2(4, 1));
	_isOneWay = false;
}

void Ladder::SetBottom()
{
	_sprite->SetCurClip(Vector2(4, 3));
	_isOneWay = false;
}

void Ladder::SetOneWay()
{
	_sprite->SetCurClip(Vector2(4, 2));
	_isOneWay = true;
}

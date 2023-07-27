#include "framework.h"
#include "Wooden.h"

Wooden::Wooden()
	:Tile()
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floorstyled_wood.png", Vector2(10, 10), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(7, 2));
	_type = Tile::Type::WOODEN;
}

Wooden::Wooden(Vector2 pos)
	:Tile(pos)
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floorstyled_wood.png", Vector2(10, 10), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(7, 2));
	_type = Tile::Type::WOODEN;
}

Wooden::~Wooden()
{
}

void Wooden::Update()
{
	Tile::Update();
}

void Wooden::Render()
{
	Tile::Render();
}

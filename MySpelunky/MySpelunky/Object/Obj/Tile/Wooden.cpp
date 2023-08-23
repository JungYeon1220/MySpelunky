#include "framework.h"
#include "Wooden.h"

Wooden::Wooden()
	:Tile()
{
	_name = "Wooden";
	_type = Tile::Type::WOODEN;
}

Wooden::Wooden(Vector2 pos)
	:Tile(pos)
{
	_name = "Wooden";
	_type = Tile::Type::WOODEN;
}

Wooden::~Wooden()
{
}

void Wooden::Update()
{
	_col->Update();
	_transform->Update();
}

void Wooden::Render()
{
	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Wood", _name);
}

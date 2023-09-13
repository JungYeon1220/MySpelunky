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
	if (_isActive == false)
		return;
	_col->Update();
	_transform->Update();
}

void Wooden::Render()
{
	if (_isActive == false)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Wood", _name);
}

#include "framework.h"
#include "OneWay.h"

OneWay::OneWay()
	:Tile()
{
	_name = "Oneway";
	_type = Tile::Type::ONE_WAY;
}

OneWay::OneWay(Vector2 pos)
	:Tile(pos)
{
	_name = "Oneway";
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


	if ((colPos.x <= pos.x + halfSize.x && colPos.x >= pos.x - halfSize.x)
		&& (colPos.y >= pos.y + halfSize.y))
	{
		return _col->Block(col);
	}

	return false;
}

void OneWay::Update()
{
	_col->Update();
	_transform->Update();
}

void OneWay::Render()
{
	_transform->SetWorldBuffer(0);
	TILEMANAGER->Render("FloorMisc", _name);
}

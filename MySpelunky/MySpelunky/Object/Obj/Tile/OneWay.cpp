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
	_transform->SetPosition(Vector2(0.0f, 0.0f));
	_transform->Update();
	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("FloorMisc", _name);

	for (int i = 0; i < _legCount; i++)
	{
		_transform->SetPosition(Vector2(0.0f, -100.0f * (i + 1)));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		if (i == _legCount - 1)
			SPRITEMANAGER->Render("FloorMisc", "OnewayBottom");
		else
			SPRITEMANAGER->Render("FloorMisc", "OnewayLeg");
	}
}

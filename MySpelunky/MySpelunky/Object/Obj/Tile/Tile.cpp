#include "framework.h"
#include "Tile.h"
#include "../Player.h"

Tile::Tile()
{
	_transform = make_shared<Transform>();
	_col = make_shared<RectCollider>(Vector2(100.0f, 100.0f));
	_transform->SetParent(_col->GetTransform());
	_col->GetTransform()->SetPosition(_pos);
}

Tile::Tile(Vector2 pos)
	:_pos(pos)
{
	_transform = make_shared<Transform>();
	_col = make_shared<RectCollider>(Vector2(100.0f, 100.0f));
	_transform->SetParent(_col->GetTransform());
	_col->GetTransform()->SetPosition(_pos);

}

Tile::~Tile()
{
}

void Tile::SetType(Tile::Type value)
{
	_type = value;
}

bool Tile::Block(shared_ptr<Collider> col)
{
	if (_isActive == false)
		return false;
	if (_type == Type::EMPTY)
		return false;

	Vector2 colPos = col->GetWorldPos();
	Vector2 pos = _col->GetWorldPos();
	Vector2 halfSize = Vector2(50.0f, 50.0f);

	if ((colPos.x <= pos.x + halfSize.x && colPos.x >= pos.x - halfSize.x)
	|| (colPos.y <= pos.y + halfSize.y && colPos.y >= pos.y - halfSize.y))
	{
		return _col->Block(col);
	}

	return false;
}

void Tile::InteractPlayer(shared_ptr<Player> player)
{
}

void Tile::Update()
{
	if (_isActive == false)
		return;
	if (_type == EMPTY)
		return;

	_transform->Update();
	_col->Update();
}

void Tile::Render()
{
	if (_isActive == false)
		return;
	if (_type == EMPTY)
		return;

	_transform->SetWorldBuffer(0);
	//_col->Render();
}

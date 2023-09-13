#include "framework.h"
#include "Ladder.h"

Ladder::Ladder()
	:Tile()
{
	_name = "NormalLadder";
	_type = Tile::Type::LADDER;
}

Ladder::Ladder(Vector2 pos)
	:Tile(pos)
{
	_name = "NormalLadder";
	_type = Tile::Type::LADDER;
}

Ladder::~Ladder()
{
}

bool Ladder::Block(shared_ptr<Collider> col)
{
	if (_isActive == false)
		return false;

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
	if (_isActive == false)
		return;
	Tile::Update();
}

void Ladder::Render()
{
	if (_isActive == false)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Cave", _name);
}

void Ladder::SetTop()
{
	_name = "TopLadder";
	_isOneWay = false;
}

void Ladder::SetMiddle()
{
	_name = "NormalLadder";
	_isOneWay = false;
}

void Ladder::SetBottom()
{
	_name = "BottomLadder";
	_isOneWay = false;
}

void Ladder::SetOneWay()
{
	_name = "PlatformLadder";
	_isOneWay = true;
}

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
	if (_isActive == false)
		return false;
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

void OneWay::InteractPlayer(shared_ptr<class Player> player)
{
	if (_isActive == false)
		return;

	Vector2 playerIdx = MathUtility::GetGridIndex(player->GetCollider()->GetWorldPos());
	if (_index.x > playerIdx.x + 1 || _index.x < playerIdx.x - 1)
		return;
	if (_index.y > playerIdx.y + 1 || _index.y < playerIdx.y - 1)
		return;

	if ((player->GetJumpPower() <= 0.0f) && player->IsClimb() == false)
	{
		if (Block(player->GetCollider()))
		{
			player->IsFalling() = false;
			player->IsOnOneWay() = true;

			if (KEY_PRESS(VK_DOWN) && KEY_DOWN('Z'))
			{
				_canStand = false;
			}
		}

		if (_col->IsCollision(player->GetCollider()) == false)
			_canStand = true;
	}
}

void OneWay::Update()
{
	if (_isActive == false)
		return;
	_col->Update();
	_transform->Update();
}

void OneWay::Render()
{
	if (_isActive == false)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == true)
	{
		_transform->SetPosition(Vector2(0.0f, 0.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("FloorMisc", _name);
	}

	for (int i = 0; i < _legCount; i++)
	{
		_transform->SetPosition(Vector2(0.0f, -100.0f * (i + 1)));
		if (CAMERA->GetViewCollider()->GetWorldPos().y + WIN_HEIGHT * CAMERA->_invScale.y * 0.5f < _transform->GetWorldPos().y - 50.0f
			|| CAMERA->GetViewCollider()->GetWorldPos().y - WIN_HEIGHT * CAMERA->_invScale.y * 0.5f > _transform->GetWorldPos().y + 50.0f
			|| CAMERA->GetViewCollider()->GetWorldPos().x + WIN_WIDTH * CAMERA->_invScale.x * 0.5f < _transform->GetWorldPos().x - 50.0f
			|| CAMERA->GetViewCollider()->GetWorldPos().x - WIN_WIDTH * CAMERA->_invScale.x * 0.5f > _transform->GetWorldPos().x + 50.0f)
			continue;
		_transform->Update();
		_transform->SetWorldBuffer(0);
		if (i == _legCount - 1)
			SPRITEMANAGER->Render("FloorMisc", "OnewayBottom");
		else
			SPRITEMANAGER->Render("FloorMisc", "OnewayLeg");
	}
}

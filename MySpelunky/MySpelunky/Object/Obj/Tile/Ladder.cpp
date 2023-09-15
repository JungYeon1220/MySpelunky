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

void Ladder::InteractPlayer(shared_ptr<class Player> player)
{
	if (_isActive == false)
		return;

	Vector2 playerIdx = MathUtility::GetGridIndex(player->GetCollider()->GetWorldPos());

	if (_index.x > playerIdx.x + 1 || _index.x < playerIdx.x - 1)
		return;
	if (_index.y > playerIdx.y + 1 || _index.y < playerIdx.y - 1)
		return;
	float tileX = _col->GetWorldPos().x;
	float tileY = _col->GetWorldPos().y;
	float playerX = player->GetCollider()->GetWorldPos().x;
	float playerY = player->GetCollider()->GetWorldPos().y;
	float playerXHalfSize = player->GetSize().x * 0.5f;
	float playerYHalfSize = player->GetSize().y * 0.5f;

	if (_name == "BottomLadder")
	{
		if (tileY - 50.0f > playerY + playerYHalfSize && player->IsRope() == false)
			player->IsClimb() = false;
	}

	if (_col->IsCollision(player->GetCollider()))
	{
		if (tileX < playerX + playerXHalfSize && tileX > playerX - playerXHalfSize)
		{
			if (KEY_DOWN(VK_UP) || KEY_PRESS(VK_UP))
			{
				if (player->CanClimb() == true)
				{
					player->GetCollider()->GetTransform()->SetPosition(Vector2(tileX, playerY));
					if (_name == "TopLadder")
					{
						if (player->GetCollider()->GetWorldPos().y >= _col->GetWorldPos().y)
							player->GetCollider()->GetTransform()->SetPosition(Vector2(playerX, tileY));
					}
				}
				player->IsClimb() = true;
				player->IsRope() = false;
			}
		}
	}

	if (player->IsClimb() == false)
	{
		if (_isOneWay == true)
		{
			if ((player->GetJumpPower() <= 0.0f))
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
	}
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

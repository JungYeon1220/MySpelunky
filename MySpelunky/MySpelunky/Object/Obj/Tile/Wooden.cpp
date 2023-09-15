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

void Wooden::InteractPlayer(shared_ptr<class Player> player)
{
	if (_isActive == false)
		return;

	Vector2 playerIdx = MathUtility::GetGridIndex(player->GetCollider()->GetWorldPos());

	if (_index.x > playerIdx.x + 1 || _index.x < playerIdx.x - 1)
		return;
	if (_index.y > playerIdx.y + 1 || _index.y < playerIdx.y - 1)
		return;

	if (Block(player->GetCollider()))
	{
		if (_col->IsCollision(player->GetFeetCollider()))
			player->IsFalling() = false;
		if (_col->IsCollision(player->GetHeadCollider()))
			player->GetJumpPower() = 0.0f;

		Vector2 tilePos = _col->GetWorldPos();

		if (player->_hasGloves == true)
		{
			if (player->GetGrabCollider()->IsCollision(_col))
			{
				if (player->GetJumpPower() < 0.0f)
				{
					if (KEY_PRESS(VK_LEFT) || KEY_PRESS(VK_RIGHT))
					{
						player->IsGrab() = true;
					}
				}
			}
		}
		else
		{
			if (_canGrab == true)
			{
				if (player->GetGrabCollider()->IsCollision(tilePos + Vector2(50.0f, 50.0f)) 
				 || player->GetGrabCollider()->IsCollision(tilePos + Vector2(-50.0f, 50.0f)))
				{
					if (player->GetJumpPower() <= 0.0f)
					{
						if (KEY_PRESS(VK_LEFT) || KEY_PRESS(VK_RIGHT))
						{
							player->GetCollider()->GetTransform()->SetPosition(Vector2(player->GetCollider()->GetWorldPos().x, tilePos.y + 20.0f));
							player->IsGrab() = true;
						}
					}
				}
			}
		}
	}
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

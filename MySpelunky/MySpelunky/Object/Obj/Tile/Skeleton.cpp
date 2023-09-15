#include "framework.h"
#include "Skeleton.h"

Skeleton::Skeleton()
	:Tile()
{
	_name = "Skeleton";
	_type = Tile::Type::SKELETON;
}

Skeleton::Skeleton(Vector2 pos)
	:Tile(pos)
{
	_name = "Skeleton";
	_type = Tile::Type::SKELETON;
}

Skeleton::~Skeleton()
{
}

void Skeleton::Update()
{
	if (_isActive == false)
		return;
	_col->Update();
}

void Skeleton::Render()
{
	if (_isActive == false)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

	_transform->SetPosition(Vector2(0, 0));
	_transform->Update();
	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Cave", _name);
	if (_upPebble == true)
	{
		_transform->SetPosition(Vector2(0.0f, 50.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("Cave", _pebbleName);
	}
}

void Skeleton::InteractPlayer(shared_ptr<class Player> player)
{
	if (_isActive == false)
		return;

	Vector2 playerIdx = MathUtility::GetGridIndex(player->GetCollider()->GetWorldPos());

	if (_index.x > playerIdx.x + 1 || _index.x < playerIdx.x - 1)
		return;
	if (_index.y > playerIdx.y + 1 || _index.y < playerIdx.y - 1)
		return;

	if (player->GetWhip()->IsActive() == true && player->GetWhip()->GetCollider()->IsCollision(_col))
		_isActive = false;

	if (Block(player->GetCollider()))
	{
		if (_col->IsCollision(player->GetFeetCollider()))
			player->IsFalling() = false;
		if (_col->IsCollision(player->GetHeadCollider()))
			player->GetJumpPower() = 0.0f;

		Vector2 tilePos = _col->GetWorldPos();

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

void Skeleton::SetSpikePebble()
{
	_upPebble = true;
	_pebbleName = "SkeletonSpikePebble";
}

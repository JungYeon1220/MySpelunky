#include "framework.h"
#include "Normal.h"
#include "../Player.h"

Normal::Normal()
	:Tile()
{
	_name = "Normal" + to_string(MathUtility::RandomInt(1, 4));
	_type = Tile::Type::NORMAL;
}

Normal::Normal(Vector2 pos)
	: Tile(pos)
{
	_name = "Normal" + to_string(MathUtility::RandomInt(1, 4));
	_type = Tile::Type::NORMAL;
}

Normal::~Normal()
{
}

void Normal::Update()
{
	if (_isActive == false)
		return;
	_col->Update();
	_transform->Update();
}

void Normal::Render()
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
		SPRITEMANAGER->Render("Cave", "UpPebble" + to_string(_upPebbleNum));
	}
	if (_downPebble == true)
	{
		_transform->SetPosition(Vector2(0.0f, -50.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("Cave", "DownPebble" + to_string(_downPebbleNum));
	}
	if (_rightPebble == true)
	{
		_transform->SetPosition(Vector2(50.0f, 0.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("Cave", "SidePebble" + to_string(_rightPebbleNum));
	}
	if (_leftPebble == true)
	{
		_transform->SetPosition(Vector2(-50.0f, 0.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->GetSprite("Cave")->SetLeft();
		SPRITEMANAGER->Render("Cave", "SidePebble" + to_string(_leftPebbleNum));
		SPRITEMANAGER->GetSprite("Cave")->SetRight();
	}
}

void Normal::Destroy()
{
	_isActive = false;
}

void Normal::InteractPlayer(shared_ptr<Player> player)
{
	Vector2 playerIdx = MathUtility::GetGridIndex(player->GetCollider()->GetWorldPos());
	if (_index.x > playerIdx.x + 1 || _index.x < playerIdx.x - 1)
		return;
	if (_index.y > playerIdx.y + 1 || _index.y < playerIdx.y - 1)
		return;

	if (Block(player->GetCollider()))
	{
		if (_col->IsCollision(player->GetFeetCollider()))
			player->IsFalling() = false;
		else if (_col->IsCollision(player->GetHeadCollider()))
			player->GetJumpPower() = 0.0f;
		else
			player->GetSpeed() = 0.0f;

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

void Normal::SetSpikePebble()
{
	_upPebble = true;
	_upPebbleNum = MathUtility::RandomInt(4, 6);
}

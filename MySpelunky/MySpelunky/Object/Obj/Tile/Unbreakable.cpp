#include "framework.h"
#include "Unbreakable.h"
#include "../Player.h"

Unbreakable::Unbreakable()
	:Tile()
{
	_name = "Unbreakable";
	_type = Tile::Type::UNBREAKABLE;
}

Unbreakable::Unbreakable(Vector2 pos)
	:Tile(pos)
{
	_name = "Unbreakable";
	_type = Tile::Type::UNBREAKABLE;
}

Unbreakable::~Unbreakable()
{
}

void Unbreakable::InteractPlayer(shared_ptr<class Player> player)
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

		if (_col->IsCollision(player->GetHeadCollider()))
			player->GetJumpPower() = 0.0f;

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
	}
}

void Unbreakable::SetRandomTile(int x, int y)
{
	int m = x % 2;
	int n = y % 2;
	int random = (rand() % 3) * 2;

	_name += to_string(random);

	if (m == 0 && n == 0)
		_name += to_string(1);
	else if (m == 0 && n == 1)
		_name += to_string(2);
	else if (m == 1 && n == 0)
		_name += to_string(3);
	else
		_name += to_string(4);
}

void Unbreakable::Update()
{
	if (_isActive == false)
		return;
	_col->Update();
}

void Unbreakable::Render()
{
	if (_isActive == false)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

	_transform->SetPosition(Vector2(0, 0));
	_transform->Update();
	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("BorderMain", _name);

	if (_upPebble == true)
	{
		_transform->SetPosition(Vector2(0.0f, 55.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("BorderMain", "UBUpPebble" + to_string(_upPebbleNum));
	}
	if (_downPebble == true)
	{
		_transform->SetPosition(Vector2(0.0f, -55.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("BorderMain", "UBDownPebble" + to_string(_downPebbleNum));
	}
	if (_rightPebble == true)
	{
		_transform->SetPosition(Vector2(50.0f, 0.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("BorderMain", "UBSidePebble" + to_string(_rightPebbleNum));
	}
	if (_leftPebble == true)
	{
		_transform->SetPosition(Vector2(-50.0f, 0.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->GetSprite("BorderMain")->SetLeft();
		SPRITEMANAGER->Render("BorderMain", "UBSidePebble" + to_string(_leftPebbleNum));
		SPRITEMANAGER->GetSprite("BorderMain")->SetRight();
	}
}

#include "framework.h"
#include "TileTestScene.h"
#include "../../Object/Obj/Tile/Normal.h"
#include "../../Object/Obj/Tile/Ladder.h"
#include "../../Object/Obj/Tile/OneWay.h"
#include "../../Object/Obj/Tile/Unbreakable.h"
#include "../../Object/Obj/Tile/Wooden.h"
#include "../../Object/Obj/Tile/Skeleton.h"
#include "../../Object/Obj/Tile/Spike.h"
#include "../../Object/Obj/Tile/Movable.h"
#include "../../Object/Obj/Map.h"
#include "../../Object/Obj/Player.h"
#include "../../Object/Obj/Monster/Monster.h"
#include "../../Object/Obj/Monster/Mosquito.h"
#include "../../Object/Obj/Monster/Snake.h"
#include "../../Object/Obj/Monster/Spider.h"

TileTestScene::TileTestScene()
{
	_map = make_shared<Map>();
	_player = make_shared<Player>();
	_player->GetCollider()->GetTransform()->SetPosition(Vector2(_map->GetStartPos().x * 100.0f, (_map->PoolCount().y - 1 - _map->GetStartPos().y) * 100.0f));

	for (int i = 0; i < _map->PoolCount().y; i++)
	{
		for (int j = 0; j < _map->PoolCount().x; j++)
		{
			if (_map->GetTiles()[i][j] == nullptr)
				continue;

			if (_map->GetTiles()[i][j]->GetType() == Tile::Type::NORMAL)
			{
				if (_map->GetTiles()[i - 1][j] == nullptr)
				{
					int random = MathUtility::RandomInt(0, 30);

					if (random < 3)
					{
						shared_ptr<Monster> monster;

						if (random == 0)
							monster = make_shared<Spider>(_map->GetTiles()[i][j]->GetCollider()->GetWorldPos() + Vector2(0.0f, 100.0f));
						else if (random == 1)
							monster = make_shared<Snake>(_map->GetTiles()[i][j]->GetCollider()->GetWorldPos() + Vector2(0.0f, 100.0f));
						else if (random == 2)
							monster = make_shared<Mosquito>(_map->GetTiles()[i][j]->GetCollider()->GetWorldPos() + Vector2(0.0f, 100.0f));

						_monsters.push_back(monster);
					}

				}
			}
		}
	}

	CAMERA->SetPosition(_player->GetCollider()->GetWorldPos());
	CAMERA->GetViewCollider()->GetTransform()->SetPosition(_player->GetCollider()->GetWorldPos());
	CAMERA->SetTarget(_player->GetCollider()->GetTransform());
	CAMERA->SetLeftBottom(_map->GetTiles()[_map->PoolCount().y - 1][0]->GetCollider()->GetWorldPos());
	CAMERA->SetRightTop(_map->GetTiles()[0][_map->PoolCount().x - 1]->GetCollider()->GetWorldPos());
	 
	//CAMERA->FreeMode();
	//CAMERA->SetScale(Vector2(0.5f, 0.5f));
}

TileTestScene::~TileTestScene()
{
}

void TileTestScene::Update()
{
	_player->Update();
	_map->Update();
	if (m)
	{
		for (auto monster : _monsters)
			monster->Update();
	}

	bool check = false;
	bool ladderCheck = false;
	_player->IsOnOneWay() = false;

	for (auto normal : _map->GetTypeTiles()["Normal"])
	{
		float x = _player->GetCollider()->GetWorldPos().x - normal->GetCollider()->GetWorldPos().x;
		float y = _player->GetCollider()->GetWorldPos().y - normal->GetCollider()->GetWorldPos().y;
		if (x * x + y * y > 40000.0f)
			continue;

		if (normal->Block(_player->GetCollider()))
		{
			if (normal->GetCollider()->IsCollision(_player->GetFeetCollider()))
				check = true;
			if (normal->GetCollider()->IsCollision(_player->GetHeadCollider()))
				_player->GetJumpPower() = 0.0f;

			Vector2 tilePos = normal->GetCollider()->GetWorldPos();

			if (dynamic_pointer_cast<Normal>(normal)->CanGrab() == true)
			{
				if (_player->GetGrabCollider()->IsCollision(tilePos + Vector2(50.0f, 50.0f)) || _player->GetGrabCollider()->IsCollision(tilePos + Vector2(-50.0f, 50.0f)))
				{
					if (_player->GetJumpPower() <= 0.0f)
					{
						if (KEY_PRESS(VK_LEFT) || KEY_PRESS(VK_RIGHT))
						{
							_player->GetCollider()->GetTransform()->SetPosition(Vector2(_player->GetCollider()->GetWorldPos().x, tilePos.y + 20.0f));
							_player->IsGrab() = true;
						}
					}
				}
			}
		}
	}
	for (auto unbreakable : _map->GetTypeTiles()["Unbreakable"])
	{
		if (unbreakable->Block(_player->GetCollider()))
		{
			float x = _player->GetCollider()->GetWorldPos().x - unbreakable->GetCollider()->GetWorldPos().x;
			float y = _player->GetCollider()->GetWorldPos().y - unbreakable->GetCollider()->GetWorldPos().y;
			if (x * x + y * y > 40000.0f)
				continue;

			if (unbreakable->GetCollider()->IsCollision(_player->GetFeetCollider()))
				check = true;
			if (unbreakable->GetCollider()->IsCollision(_player->GetHeadCollider()))
				_player->GetJumpPower() = 0.0f;
		}
	}
	for (auto oneway : _map->GetTypeTiles()["Oneway"])
	{
		float x = _player->GetCollider()->GetWorldPos().x - oneway->GetCollider()->GetWorldPos().x;
		float y = _player->GetCollider()->GetWorldPos().y - oneway->GetCollider()->GetWorldPos().y;
		if (x * x + y * y > 40000.0f)
			continue;

		if ((_player->GetJumpPower() <= 0.0f))
		{
			if (oneway->Block(_player->GetCollider()))
			{
				check = true;
				_player->IsOnOneWay() = true;

				if (KEY_PRESS(VK_DOWN) && KEY_DOWN('Z'))
				{
					dynamic_pointer_cast<OneWay>(oneway)->CanStand() = false;
				}
			}

			if (oneway->GetCollider()->IsCollision(_player->GetCollider()) == false)
				dynamic_pointer_cast<OneWay>(oneway)->CanStand() = true;
		}
	}
	for (auto ladder : _map->GetTypeTiles()["Ladder"])
	{
		float x = _player->GetCollider()->GetWorldPos().x - ladder->GetCollider()->GetWorldPos().x;
		float y = _player->GetCollider()->GetWorldPos().y - ladder->GetCollider()->GetWorldPos().y;
		if (x * x + y * y > 40000.0f)
			continue;

		if (ladder->GetCollider()->IsCollision(_player->GetCollider()))
		{
			float tileX = ladder->GetCollider()->GetWorldPos().x;
			float playerX = _player->GetCollider()->GetWorldPos().x;
			float playerXHalfSize = _player->GetSize().x * 0.5f;
			if (tileX < playerX + playerXHalfSize && tileX > playerX - playerXHalfSize)
			{
				if (KEY_DOWN(VK_UP) || KEY_PRESS(VK_UP))
				{
					if (_player->CanClimb() == true)
						_player->GetCollider()->GetTransform()->SetPosition(Vector2(ladder->GetCollider()->GetWorldPos().x, _player->GetCollider()->GetWorldPos().y));
					_player->IsClimb() = true;
				}
				ladderCheck = true;
			}
		}
		if (_player->IsClimb() == false)
		{
			if (dynamic_pointer_cast<Ladder>(ladder)->IsOneWay() == true)
			{
				if ((_player->GetJumpPower() <= 0.0f))
				{
					if (ladder->Block(_player->GetCollider()))
					{
						check = true;
						_player->IsOnOneWay() = true;

						if (KEY_PRESS(VK_DOWN) && KEY_DOWN('Z'))
						{
							dynamic_pointer_cast<Ladder>(ladder)->CanStand() = false;
						}
					}

					if (ladder->GetCollider()->IsCollision(_player->GetCollider()) == false)
						dynamic_pointer_cast<Ladder>(ladder)->CanStand() = true;
				}
			}
		}
	}
	for (auto wooden : _map->GetTypeTiles()["Wooden"])
	{
		float x = _player->GetCollider()->GetWorldPos().x - wooden->GetCollider()->GetWorldPos().x;
		float y = _player->GetCollider()->GetWorldPos().y - wooden->GetCollider()->GetWorldPos().y;
		if (x * x + y * y > 40000.0f)
			continue;

		if (wooden->Block(_player->GetCollider()))
		{
			if (wooden->GetCollider()->IsCollision(_player->GetFeetCollider()))
				check = true;
			if (wooden->GetCollider()->IsCollision(_player->GetHeadCollider()))
				_player->GetJumpPower() = 0.0f;

			Vector2 tilePos = wooden->GetCollider()->GetWorldPos();

			if (dynamic_pointer_cast<Wooden>(wooden)->CanGrab() == true)
			{
				if (_player->GetGrabCollider()->IsCollision(tilePos + Vector2(50.0f, 50.0f)) || _player->GetGrabCollider()->IsCollision(tilePos + Vector2(-50.0f, 50.0f)))
				{
					if (_player->GetJumpPower() <= 0.0f)
					{
						if (KEY_PRESS(VK_LEFT) || KEY_PRESS(VK_RIGHT))
						{
							_player->GetCollider()->GetTransform()->SetPosition(Vector2(_player->GetCollider()->GetWorldPos().x, tilePos.y + 20.0f));
							_player->IsGrab() = true;
						}
					}
				}
			}
		}
	}
	for (auto skeleton : _map->GetTypeTiles()["Skeleton"])
	{
		float x = _player->GetCollider()->GetWorldPos().x - skeleton->GetCollider()->GetWorldPos().x;
		float y = _player->GetCollider()->GetWorldPos().y - skeleton->GetCollider()->GetWorldPos().y;
		if (x * x + y * y > 40000.0f)
			continue;

		if (skeleton->Block(_player->GetCollider()))
		{
			if (skeleton->GetCollider()->IsCollision(_player->GetFeetCollider()))
				check = true;
			if (skeleton->GetCollider()->IsCollision(_player->GetHeadCollider()))
				_player->GetJumpPower() = 0.0f;

			Vector2 tilePos = skeleton->GetCollider()->GetWorldPos();

			if (dynamic_pointer_cast<Skeleton>(skeleton)->CanGrab() == true)
			{
				if (_player->GetGrabCollider()->IsCollision(tilePos + Vector2(50.0f, 50.0f)) || _player->GetGrabCollider()->IsCollision(tilePos + Vector2(-50.0f, 50.0f)))
				{
					if (_player->GetJumpPower() <= 0.0f)
					{
						if (KEY_PRESS(VK_LEFT) || KEY_PRESS(VK_RIGHT))
						{
							_player->GetCollider()->GetTransform()->SetPosition(Vector2(_player->GetCollider()->GetWorldPos().x, tilePos.y + 20.0f));
							_player->IsGrab() = true;
						}
					}
				}
			}
		}
	}
	for (auto spike : _map->GetTypeTiles()["Spike"])
	{
		float x = _player->GetCollider()->GetWorldPos().x - spike->GetCollider()->GetWorldPos().x;
		float y = _player->GetCollider()->GetWorldPos().y - spike->GetCollider()->GetWorldPos().y;
		if (x * x + y * y > 40000.0f)
			continue;

		if ((_player->GetJumpPower() < 0.0f))
		{
			if (spike->GetCollider()->IsCollision(_player->GetCollider()) == true)
			{
				if (_player->GetCollider()->GetWorldPos().y > spike->GetCollider()->GetWorldPos().y)
				{
					if (_player->GetCollider()->GetWorldPos().x >= spike->GetCollider()->GetWorldPos().x - 50.0f && _player->GetCollider()->GetWorldPos().x <= spike->GetCollider()->GetWorldPos().x + 50.0f)
					{
						if (dynamic_pointer_cast<Spike>(spike)->CanSpike() == true)
						{
							_player->Dead();
							dynamic_pointer_cast<Spike>(spike)->SetBlood();
							spike->Update();
						}

						dynamic_pointer_cast<Spike>(spike)->CanSpike() = false;
					}
				}
			}

			if (spike->GetCollider()->IsCollision(_player->GetCollider()) == false)
				dynamic_pointer_cast<Spike>(spike)->CanSpike() = true;
		}
	}
	for (auto movable : _map->GetTypeTiles()["Movable"])
	{
		float x = _player->GetCollider()->GetWorldPos().x - movable->GetCollider()->GetWorldPos().x;
		float y = _player->GetCollider()->GetWorldPos().y - movable->GetCollider()->GetWorldPos().y;
		if (x * x + y * y > 40000.0f)
			continue;

		if (movable->Block(_player->GetCollider()))
		{
			if (movable->GetCollider()->IsCollision(_player->GetFeetCollider()))
				check = true;
			if (movable->GetCollider()->IsCollision(_player->GetHeadCollider()))
				_player->GetJumpPower() = 0.0f;

			Vector2 tilePos = movable->GetCollider()->GetWorldPos();

			if (dynamic_pointer_cast<Movable>(movable)->CanGrab() == true)
			{
				if (_player->GetGrabCollider()->IsCollision(tilePos + Vector2(50.0f, 50.0f)) || _player->GetGrabCollider()->IsCollision(tilePos + Vector2(-50.0f, 50.0f)))
				{
					if (_player->GetJumpPower() <= 0.0f)
					{
						if (KEY_PRESS(VK_LEFT) || KEY_PRESS(VK_RIGHT))
						{
							_player->GetCollider()->GetTransform()->SetPosition(Vector2(_player->GetCollider()->GetWorldPos().x, tilePos.y + 20.0f));
							_player->IsGrab() = true;
						}
					}
				}
			}
			if (dynamic_pointer_cast<Movable>(movable)->IsFalling() == false)
			{
				if (_player->GetCollider()->GetWorldPos().y < movable->GetCollider()->GetWorldPos().y + 50.0f && _player->GetCollider()->GetWorldPos().y > movable->GetCollider()->GetWorldPos().y - 50.0f)
				{
					if (_player->IsFalling() == false)
					{
						if (KEY_PRESS(VK_LEFT))
						{
							movable->GetCollider()->GetTransform()->AddVector2(-RIGHT_VECTOR * 100.0f * DELTA_TIME);
							_player->IsPush() = true;
						}

						if (KEY_PRESS(VK_RIGHT))
						{
							movable->GetCollider()->GetTransform()->AddVector2(RIGHT_VECTOR * 100.0f * DELTA_TIME);
							_player->IsPush() = true;

						}
					}
				}
			}
		}
		else
			_player->IsPush() = false;

	}

	if (check == false)
	{
		_player->IsFalling() = true;
	}
	else
	{
		_player->IsFalling() = false;
		if (KEY_PRESS(VK_DOWN))
			_player->IsClimb() = false;
	}

	if (ladderCheck == false)
		_player->IsClimb() = false;

	for (auto monster : _monsters)
	{
		bool check = false;
		for (auto tiles : _map->GetTiles())
		{
			for (auto tile : tiles)
			{
				if (tile == nullptr)
					continue;

				float x = monster->GetCollider()->GetWorldPos().x - tile->GetCollider()->GetWorldPos().x;
				float y = monster->GetCollider()->GetWorldPos().y - tile->GetCollider()->GetWorldPos().y;
				if (x * x + y * y > 40000.0f)
					continue;

				if (monster->TileInteract(tile) == true)
					check = true;
			}
		}

		if (_player->GetWhip()->IsActive() == true)
		{
			if (_player->GetWhip()->GetCollider()->IsCollision(monster->GetCollider()))
			{
				monster->TakeDamage(1);
			}
		}


		if (monster->IsDead() == false)
		{
			monster->SetTarget(_player);

			if (monster->GetCollider()->IsCollision(_player->GetHitCollider()))
			{
				if (monster->GetCollider()->IsCollision(_player->GetFeetCollider()))
				{
					if (_player->IsFalling() == true && _player->GetJumpPower() < 0.0f)
					{
						if (KEY_PRESS('Z'))
							_player->GetJumpPower() = 1200.0f;
						else
							_player->GetJumpPower() = 800.0f;
						monster->TakeDamage(1);
					}
				}
				else
				{
					_player->KnockBack(monster->GetCollider()->GetWorldPos(), 300.0f);
					_player->TakeDamage(0);
				}
			}
		}

		monster->Land(check);
	}

	m = true;
}

void TileTestScene::Render()
{
	_map->BehindRender();
	_player->Render();
	for (auto monster : _monsters)
		monster->Render();
	_map->FrontRender();
}

void TileTestScene::PostRender()
{
	_player->PostRender();
	_map->PostRender();
}

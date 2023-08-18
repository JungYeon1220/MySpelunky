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
	_map->Update();
	_player = make_shared<Player>();
	_player->GetCollider()->GetTransform()->SetPosition(Vector2(_map->GetStartPos().x * 100.0f, (_map->PoolCount().y - 1 - _map->GetStartPos().y) * 100.0f));

	for (int i = 0; i < _map->PoolCount().y; i++)
	{
		for (int j = 0; j < _map->PoolCount().x; j++)
		{
			if (_map->GetTiles()[i][j]->GetType() == Tile::Type::NORMAL)
			{
				if (_map->GetTiles()[i - 1][j]->GetType() == Tile::Type::EMPTY)
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

	//CAMERA->SetTarget(_player->GetCollider()->GetTransform());
	//CAMERA->SetLeftBottom(_map->GetTiles()[_map->PoolCount().y - 1][0]->GetCollider()->GetWorldPos());
	//CAMERA->SetRightTop(_map->GetTiles()[0][_map->PoolCount().x - 1]->GetCollider()->GetWorldPos());
	 
	CAMERA->FreeMode();
	CAMERA->SetScale(Vector2(0.3f, 0.3f));
}

TileTestScene::~TileTestScene()
{
}

void TileTestScene::Update()
{
	_player->Update();
	_map->Update();
	for (auto monster : _monsters)
		monster->Update();

	bool check = false;
	bool ladderCheck = false;
	_player->IsOnOneWay() = false;

	for (auto tileArr : _map->GetTiles())
	{
		for (auto tile : tileArr)
		{
			if (tile == nullptr)
				continue;

			float x = _player->GetCollider()->GetWorldPos().x - tile->GetCollider()->GetWorldPos().x;
			float y = _player->GetCollider()->GetWorldPos().y - tile->GetCollider()->GetWorldPos().y;
			if (x * x + y * y > 40000.0f)
				continue;

			switch (tile->GetType())
			{
			case Tile::Type::NORMAL:
			{
				if (tile->Block(_player->GetCollider()))
				{
					if (tile->GetCollider()->IsCollision(_player->GetFeetCollider()))
						check = true;
					if (tile->GetCollider()->IsCollision(_player->GetHeadCollider()))
						_player->GetJumpPower() = 0.0f;

					Vector2 tilePos = tile->GetCollider()->GetWorldPos();

					if (dynamic_pointer_cast<Normal>(tile)->CanGrab() == true)
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
			break;
			case Tile::Type::UNBREAKABLE:
			{
				if (tile->Block(_player->GetCollider()))
				{
					if (tile->GetCollider()->IsCollision(_player->GetFeetCollider()))
						check = true;
					if (tile->GetCollider()->IsCollision(_player->GetHeadCollider()))
						_player->GetJumpPower() = 0.0f;
				}
			}
			break;
			case Tile::Type::ONE_WAY:
			{
				if ((_player->GetJumpPower() <= 0.0f))
				{
					if (tile->Block(_player->GetCollider()))
					{
						check = true;
						_player->IsOnOneWay() = true;

						if (KEY_PRESS(VK_DOWN) && KEY_DOWN('Z'))
						{
							dynamic_pointer_cast<OneWay>(tile)->CanStand() = false;
						}
					}

					if (tile->GetCollider()->IsCollision(_player->GetCollider()) == false)
						dynamic_pointer_cast<OneWay>(tile)->CanStand() = true;
				}
			}
			break;
			case Tile::Type::LADDER:
			{
				if (tile->GetCollider()->IsCollision(_player->GetCollider()))
				{
					float tileX = tile->GetCollider()->GetWorldPos().x;
					float playerX = _player->GetCollider()->GetWorldPos().x;
					float playerXHalfSize = _player->GetSize().x * 0.5f;
					if (tileX < playerX + playerXHalfSize && tileX > playerX - playerXHalfSize)
					{
						if (KEY_DOWN(VK_UP) || KEY_PRESS(VK_UP))
						{
							if (_player->CanClimb() == true)
								_player->GetCollider()->GetTransform()->SetPosition(Vector2(tile->GetCollider()->GetWorldPos().x, _player->GetCollider()->GetWorldPos().y));
							_player->IsClimb() = true;
						}
						ladderCheck = true;
					}
				}
				if (_player->IsClimb() == false)
				{
					if (dynamic_pointer_cast<Ladder>(tile)->IsOneWay() == true)
					{
						if ((_player->GetJumpPower() <= 0.0f))
						{
							if (tile->Block(_player->GetCollider()))
							{
								check = true;
								_player->IsOnOneWay() = true;

								if (KEY_PRESS(VK_DOWN) && KEY_DOWN('Z'))
								{
									dynamic_pointer_cast<Ladder>(tile)->CanStand() = false;
								}
							}

							if (tile->GetCollider()->IsCollision(_player->GetCollider()) == false)
								dynamic_pointer_cast<Ladder>(tile)->CanStand() = true;
						}
					}
				}
			}
			break;
			case Tile::Type::WOODEN:
			{
				if (tile->Block(_player->GetCollider()))
				{
					if (tile->GetCollider()->IsCollision(_player->GetFeetCollider()))
						check = true;
					if (tile->GetCollider()->IsCollision(_player->GetHeadCollider()))
						_player->GetJumpPower() = 0.0f;

					Vector2 tilePos = tile->GetCollider()->GetWorldPos();

					if (dynamic_pointer_cast<Wooden>(tile)->CanGrab() == true)
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
			break;
			case Tile::Type::SKELETON:
			{
				if (tile->Block(_player->GetCollider()))
				{
					if (tile->GetCollider()->IsCollision(_player->GetFeetCollider()))
						check = true;
					if (tile->GetCollider()->IsCollision(_player->GetHeadCollider()))
						_player->GetJumpPower() = 0.0f;

					Vector2 tilePos = tile->GetCollider()->GetWorldPos();

					if (dynamic_pointer_cast<Skeleton>(tile)->CanGrab() == true)
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
			break;
			case Tile::Type::SPIKE:
			{
				if ((_player->GetJumpPower() < 0.0f))
				{
					if (tile->GetCollider()->IsCollision(_player->GetCollider()) == true)
					{
						if(_player->GetCollider()->GetWorldPos().y > tile->GetCollider()->GetWorldPos().y)
						{
							if (_player->GetCollider()->GetWorldPos().x >= tile->GetCollider()->GetWorldPos().x - 50.0f && _player->GetCollider()->GetWorldPos().x <= tile->GetCollider()->GetWorldPos().x + 50.0f)
							{
								if (dynamic_pointer_cast<Spike>(tile)->CanSpike() == true)
								{
									_player->Dead();
									dynamic_pointer_cast<Spike>(tile)->SetBlood();
									tile->Update();
								}

								dynamic_pointer_cast<Spike>(tile)->CanSpike() = false;
							}
						}
					}

					if (tile->GetCollider()->IsCollision(_player->GetCollider()) == false)
						dynamic_pointer_cast<Spike>(tile)->CanSpike() = true;
				}
			}
			break;
			case Tile::Type::MOVABLE:
			{
				if (tile->Block(_player->GetCollider()))
				{
					if (tile->GetCollider()->IsCollision(_player->GetFeetCollider()))
						check = true;
					if (tile->GetCollider()->IsCollision(_player->GetHeadCollider()))
						_player->GetJumpPower() = 0.0f;

					Vector2 tilePos = tile->GetCollider()->GetWorldPos();

					if (dynamic_pointer_cast<Movable>(tile)->CanGrab() == true)
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
					if (dynamic_pointer_cast<Movable>(tile)->IsFalling() == false)
					{
						if (_player->GetCollider()->GetWorldPos().y < tile->GetCollider()->GetWorldPos().y + 50.0f && _player->GetCollider()->GetWorldPos().y > tile->GetCollider()->GetWorldPos().y - 50.0f)
						{
							if (_player->IsFalling() == false)
							{
								if (KEY_PRESS(VK_LEFT))
								{
									tile->GetCollider()->GetTransform()->AddVector2(-RIGHT_VECTOR * 100.0f * DELTA_TIME);
									_player->IsPush() = true;
								}

								if (KEY_PRESS(VK_RIGHT))
								{
									tile->GetCollider()->GetTransform()->AddVector2(RIGHT_VECTOR * 100.0f * DELTA_TIME);
									_player->IsPush() = true;

								}
							}
						}
					}
				}
				else
					_player->IsPush() = false;

			}
			break;
			default:
				break;
			}
		}
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
				if (monster->TileInteract(tile) == true)
					check = true;
			}
		}
		monster->Land(check);
	}
}

void TileTestScene::Render()
{
	_map->Render();
	_player->Render();
	for (auto monster : _monsters)
		monster->Render();
}

void TileTestScene::PostRender()
{
	_player->PostRender();
	_map->PostRender();
}

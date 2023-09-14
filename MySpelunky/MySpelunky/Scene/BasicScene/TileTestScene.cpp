#include "framework.h"
#include "TileTestScene.h"

TileTestScene::TileTestScene()
{
	_map = make_shared<Map>();
	_player = make_shared<Player>();
	_player->GetCollider()->GetTransform()->SetPosition(_map->GetStartPos());

	vector<vector<int>> layout = _map->GetLayout();
	for (int i = 0; i < _map->PoolCount().y; i++)
	{
		for (int j = 0; j < _map->PoolCount().x; j++)
		{
			if (layout[i][j] == 0)
				continue;

			if (layout[i][j] == 1)
			{
				if (layout[i - 1][j] == 0)
				{
					int random = MathUtility::RandomInt(0, 30);

					if (random < 3)
					{
						shared_ptr<Monster> monster;

						if (random == 0)
							monster = make_shared<Spider>(Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));
						else if (random == 1)
							monster = make_shared<Snake>(Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));
						else if (random == 2)
							monster = make_shared<Mosquito>(Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));

						_monsters.push_back(monster);
					}

				}
			}
		}
	}

	CAMERA->SetScale(Vector2(0.8f, 0.8f));
	CAMERA->SetPosition(_player->GetCollider()->GetWorldPos() * 0.8f);
	CAMERA->GetViewCollider()->GetTransform()->SetPosition(_player->GetCollider()->GetWorldPos());
	CAMERA->SetTarget(_player->GetCollider()->GetTransform());
	CAMERA->SetLeftBottom(Vector2(0,0));
	CAMERA->SetRightTop(Vector2((_map->PoolCount().x - 1) * 100.0f, (_map->PoolCount().y - 1) * 100.0f));
	 
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
	ITEMMANAGER->Update();

	if (m)
	{
		for (auto monster : _monsters)
			monster->Update();
	}

	_player->IsFalling() = true;
	_player->IsOnOneWay() = false;
	//_player->IsClimb() = false;
	Vector2 playerGrid = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());

	vector<vector<int>> layout = _map->GetLayout();
	for (auto normal : _map->GetTypeTiles()["Normal"])
	{
		int Xindex = normal->GetIndex().x;
		int Yindex = normal->GetIndex().y;

		for (auto index : _map->_distroyedTileIndex)
		{
			if (normal->GetIndex() == index + Vector2(0, 1))
			{
				normal->PebbleUp();
				normal->CanGrab() = true;
			}
			if (normal->GetIndex() == index + Vector2(0, -1))
				normal->PebbleDown();
			if (normal->GetIndex() == index + Vector2(-1, 0))
			{
				normal->PebbleRight();
				if (normal->CanGrab() == true)
				{
					dynamic_pointer_cast<Normal>(normal)->PebbleGrabRight();
					normal->LedgeRight() = true;
				}
			}
			if (normal->GetIndex() == index + Vector2(1, 0))
			{
				normal->PebbleLeft();
				if (normal->CanGrab() == true)
				{
					dynamic_pointer_cast<Normal>(normal)->PebbleGrabLeft();
					normal->LedgeLeft() = true;
				}
			}
		}

		Vector2 playerIdx = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());
		Vector2 tileIdx = normal->GetIndex();
		if (tileIdx.x > playerIdx.x + 1 || tileIdx.x < playerIdx.x - 1)
			continue;
		if (tileIdx.y > playerIdx.y + 1 || tileIdx.y < playerIdx.y - 1)
			continue;

		if (normal->Block(_player->GetCollider()))
		{
			if (normal->GetCollider()->IsCollision(_player->GetFeetCollider()))
				_player->IsFalling() = false;
			if (normal->GetCollider()->IsCollision(_player->GetHeadCollider()))
				_player->GetJumpPower() = 0.0f;

			Vector2 tilePos = normal->GetCollider()->GetWorldPos();

			if (normal->CanGrab() == true)
			{
				if (_player->GetGrabCollider()->IsCollision(tilePos + Vector2(50.0f, 50.0f))
					|| _player->GetGrabCollider()->IsCollision(tilePos + Vector2(-50.0f, 50.0f)))
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
		Vector2 playerIdx = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());
		Vector2 tileIdx = unbreakable->GetIndex();
		if (tileIdx.x > playerIdx.x + 1 || tileIdx.x < playerIdx.x - 1)
			continue;
		if (tileIdx.y > playerIdx.y + 1 || tileIdx.y < playerIdx.y - 1)
			continue;

		if (unbreakable->Block(_player->GetCollider()))
		{

			if (unbreakable->GetCollider()->IsCollision(_player->GetFeetCollider()))
				_player->IsFalling() = false;

			if (unbreakable->GetCollider()->IsCollision(_player->GetHeadCollider()))
				_player->GetJumpPower() = 0.0f;
		}
	}
	for (auto oneway : _map->GetTypeTiles()["Oneway"])
	{
		Vector2 playerIdx = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());
		Vector2 tileIdx = oneway->GetIndex();
		if (tileIdx.x > playerIdx.x + 1 || tileIdx.x < playerIdx.x - 1)
			continue;
		if (tileIdx.y > playerIdx.y + 1 || tileIdx.y < playerIdx.y - 1)
			continue;

		if ((_player->GetJumpPower() <= 0.0f) && _player->IsClimb() == false)
		{
			if (oneway->Block(_player->GetCollider()))
			{
				_player->IsFalling() = false;
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
		Vector2 playerIdx = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());
		Vector2 tileIdx = ladder->GetIndex();
		if (tileIdx.x > playerIdx.x + 1 || tileIdx.x < playerIdx.x - 1)
			continue;
		if (tileIdx.y > playerIdx.y + 1 || tileIdx.y < playerIdx.y - 1)
			continue;
		float tileX = ladder->GetCollider()->GetWorldPos().x;
		float tileY = ladder->GetCollider()->GetWorldPos().y;
		float playerX = _player->GetCollider()->GetWorldPos().x;
		float playerY = _player->GetCollider()->GetWorldPos().y;
		float playerXHalfSize = _player->GetSize().x * 0.5f;
		float playerYHalfSize = _player->GetSize().y * 0.5f;

		if (ladder->GetName() == "BottomLadder")
		{
			if (tileY - 50.0f > playerY + playerYHalfSize)
				_player->IsClimb() = false;
		}

		if (ladder->GetCollider()->IsCollision(_player->GetCollider()))
		{
			if (tileX < playerX + playerXHalfSize && tileX > playerX - playerXHalfSize)
			{
				if (KEY_DOWN(VK_UP) || KEY_PRESS(VK_UP))
				{
					if (_player->CanClimb() == true)
					{
						_player->GetCollider()->GetTransform()->SetPosition(Vector2(tileX, playerY));
						if (ladder->GetName() == "TopLadder")
						{
							if (_player->GetCollider()->GetWorldPos().y >= ladder->GetCollider()->GetWorldPos().y)
								_player->GetCollider()->GetTransform()->SetPosition(Vector2(playerX, tileY));
						}
					}
					_player->IsClimb() = true;
					_player->IsRope() = false;
				}
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
						_player->IsFalling() = false;
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
		Vector2 playerIdx = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());
		Vector2 tileIdx = wooden->GetIndex();
		if (tileIdx.x > playerIdx.x + 1 || tileIdx.x < playerIdx.x - 1)
			continue;
		if (tileIdx.y > playerIdx.y + 1 || tileIdx.y < playerIdx.y - 1)
			continue;

		if (wooden->Block(_player->GetCollider()))
		{
			if (wooden->GetCollider()->IsCollision(_player->GetFeetCollider()))
				_player->IsFalling() = false;
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
		if (_player->GetWhip()->IsActive() == true && _player->GetWhip()->GetCollider()->IsCollision(skeleton->GetCollider()))
			skeleton->IsActive() = false;

		int Xindex = skeleton->GetIndex().x;
		int Yindex = skeleton->GetIndex().y;

		for (auto index : _map->_distroyedTileIndex)
		{
			if (skeleton->GetIndex() == index + Vector2(0, 1))
			{
				skeleton->PebbleUp();
				skeleton->CanGrab() = true;
			}
		}

		Vector2 playerIdx = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());
		Vector2 tileIdx = skeleton->GetIndex();
		if (tileIdx.x > playerIdx.x + 1 || tileIdx.x < playerIdx.x - 1)
			continue;
		if (tileIdx.y > playerIdx.y + 1 || tileIdx.y < playerIdx.y - 1)
			continue;

		if (skeleton->Block(_player->GetCollider()))
		{
			if (skeleton->GetCollider()->IsCollision(_player->GetFeetCollider()))
				_player->IsFalling() = false;
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
		if (layout[spike->GetIndex().y + 1][spike->GetIndex().x] == 0)
			spike->IsActive() = false;

		Vector2 playerIdx = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());
		Vector2 tileIdx = spike->GetIndex();
		if (tileIdx.x > playerIdx.x + 1 || tileIdx.x < playerIdx.x - 1)
			continue;
		if (tileIdx.y > playerIdx.y + 1 || tileIdx.y < playerIdx.y - 1)
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
		Vector2 playerIdx = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());
		Vector2 tileIdx = movable->GetIndex();
		if (tileIdx.x > playerIdx.x + 1 || tileIdx.x < playerIdx.x - 1)
			continue;
		if (tileIdx.y > playerIdx.y + 1 || tileIdx.y < playerIdx.y - 1)
			continue;

		if (movable->Block(_player->GetCollider()))
		{
			if (movable->GetCollider()->IsCollision(_player->GetFeetCollider()))
				_player->IsFalling() = false;
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
				if (_player->GetCollider()->GetWorldPos().y < movable->GetCollider()->GetWorldPos().y + 50.0f 
					&& _player->GetCollider()->GetWorldPos().y > movable->GetCollider()->GetWorldPos().y - 50.0f)
				{
					if (_player->IsFalling() == false)
					{
						if (KEY_PRESS(VK_LEFT) && _player->GetCollider()->GetWorldPos().x > movable->GetCollider()->GetWorldPos().x)
						{
							movable->GetCollider()->GetTransform()->AddVector2(-RIGHT_VECTOR * 100.0f * DELTA_TIME);
							_player->IsPush() = true;
						}
						else if(KEY_UP(VK_LEFT))
							_player->IsPush() = false;

						if (KEY_PRESS(VK_RIGHT) && _player->GetCollider()->GetWorldPos().x < movable->GetCollider()->GetWorldPos().x)
						{
							movable->GetCollider()->GetTransform()->AddVector2(RIGHT_VECTOR * 100.0f * DELTA_TIME);
							_player->IsPush() = true;

						}
						else if(KEY_UP(VK_RIGHT))
							_player->IsPush() = false;

					}
				}
			}
		}
		else
			_player->IsPush() = false;

	}

	_map->_distroyedTileIndex.clear();

	for (auto monster : _monsters)
	{
		bool check = false;
		for (auto pair : _map->GetTypeTiles())
		{
			for (auto tile : pair.second)
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

	{
		for (auto bomb : ITEMMANAGER->GetBombs())
		{
			if (bomb->IsActive() == false && bomb->_boom == false)
				continue;

			bomb->IsFalling() = true;
			for (auto pair : _map->GetTypeTiles())
			{
				for (auto tile : pair.second)
				{
					if (tile == nullptr)
						continue;

					if (_player->GetState() == Player::State::CRAWL || _player->GetState() == Player::State::LAY_DOWN)
					{

					}

					if (bomb->_boom == true)
					{
						if (bomb->DestroyTile(tile))
						{
							_map->GetLayout()[tile->GetIndex().y][tile->GetIndex().x] = 0;
							_map->_distroyedTileIndex.push_back(tile->GetIndex());
						}

						if (bomb->GetRangeCol()->IsCollision(_player->GetHitCollider()))
						{
							_player->KnockBack(bomb->GetCollider()->GetWorldPos(), 1000.0f);
							_player->TakeDamage(3);
						}

						for (auto monster : _monsters)
						{
							if (bomb->GetRangeCol()->IsCollision(monster->GetCollider()))
								monster->TakeDamage(3);
						}
					}

					if (tile->Block(bomb->GetCollider()))
					{
						if (bomb->GetCollider()->GetWorldPos().y + bomb->GetSize().y * 0.5f > tile->GetCollider()->GetWorldPos().y - 50.0f
						 && bomb->GetCollider()->GetWorldPos().y - bomb->GetSize().y * 0.5f < tile->GetCollider()->GetWorldPos().y + 50.0f)
						{
							bomb->GetRotation() = 0.0f;
							bomb->GetSpeed() = 0.0f;
						}
						else
						{
							bomb->IsFalling() = false;
						}
					}
				}
			}

			bomb->Update();
		}

	}

	{
		for (auto rope : ITEMMANAGER->GetRopes())
		{
			if (rope->IsActive() == false)
				continue;

			rope->Update();

			float tileX = rope->GetCollider()->GetWorldPos().x;
			float tileY = rope->GetCollider()->GetWorldPos().y;
			float playerX = _player->GetCollider()->GetWorldPos().x;
			float playerY = _player->GetCollider()->GetWorldPos().y;
			float playerXHalfSize = _player->GetSize().x * 0.5f;
			float playerYHalfSize = _player->GetSize().y * 0.5f;

			for (auto pair : _map->GetTypeTiles())
			{
				for (auto tile : pair.second)
				{
					if (tile == nullptr)
						continue;
					if (tile->GetType() == Tile::Type::ONE_WAY)
						continue;
					if (tile->Block(rope->GetCollider()))
					{
						rope->GetJumpPower() = 0.0f;
					}
				}
			}

			if (rope->IsHooked() == true && rope->DropEnd() == false)
				rope->GetLength() = _map->GetRopeLength(rope->GetCollider()->GetWorldPos());

			if (rope->GetLength() == 1)
			{
				if (rope->GetCollider()->IsCollision(_player->GetCollider()))
				{
					if (tileY > playerY + playerYHalfSize)
						_player->IsClimb() = false;
				}

			}

			for (int i = 0; i < rope->GetCurLength(); i++)
			{
				if (rope->GetColliders()[i]->IsCollision(_player->GetCollider()))
				{
					if (i == rope->GetColliders().size())
					{
						if (rope->GetColliders()[i]->GetWorldPos().y - 50.0f > playerY + playerYHalfSize)
							_player->IsClimb() = false;
					}
					if (tileX < playerX + playerXHalfSize && tileX > playerX - playerXHalfSize)
					{
						if (KEY_DOWN(VK_UP) || KEY_PRESS(VK_UP))
						{
							if (_player->CanClimb() == true)
							{
								_player->GetCollider()->GetTransform()->SetPosition(Vector2(tileX, _player->GetCollider()->GetWorldPos().y));

								if (i == 0)
								{
									if (_player->GetCollider()->GetWorldPos().y >= rope->GetCollider()->GetWorldPos().y)
										_player->GetCollider()->GetTransform()->SetPosition(Vector2(playerX, tileY));
								}
							}
							_player->IsClimb() = true;
							_player->IsRope() = true;
						}
					}
				}
			}
		}
	}

	for (auto item : ITEMMANAGER->GetItems())
	{
		if (item->IsActive() == false)
			continue;

		if (_player->GetCollider()->IsCollision(item->GetCollider()))
		{
			item->InteractPlayer(_player);
			item->IsActive() = false;
		}


		bool check = false;
		for (auto pair : _map->GetTypeTiles())
		{
			for (auto tile : pair.second)
			{
				if (tile == nullptr)
					continue;

				if (tile->Block(item->GetCollider()))
				{
					if (item->GetCollider()->GetWorldPos().y + item->GetSize().y * 0.5f > tile->GetCollider()->GetWorldPos().y - 50.0f
						&& item->GetCollider()->GetWorldPos().y - item->GetSize().y * 0.5f < tile->GetCollider()->GetWorldPos().y + 50.0f)
					{
					}
					else
					{
						check = true;
					}
				}
			}
		}

		if (check == true)
			item->IsFalling() = false;
		else
			item->IsFalling() = true;
	}

	if (KEY_DOWN('P'))
	{
		_monsters.clear();
		_map->CreateStage();
		_player->GetCollider()->GetTransform()->SetPosition(_map->GetStartPos());

		for (int i = 0; i < _map->PoolCount().y; i++)
		{
			for (int j = 0; j < _map->PoolCount().x; j++)
			{
				if (layout[i][j] == 0)
					continue;

				if (layout[i][j] == 1)
				{
					if (layout[i - 1][j] == 0)
					{
						int random = MathUtility::RandomInt(0, 30);

						if (random < 3)
						{
							shared_ptr<Monster> monster;

							if (random == 0)
								monster = make_shared<Spider>(Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));
							else if (random == 1)
								monster = make_shared<Snake>(Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));
							else if (random == 2)
								monster = make_shared<Mosquito>(Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));

							_monsters.push_back(monster);
						}

					}
				}
			}
		}
	}

	m = true;

	if (KEY_DOWN(VK_LBUTTON))
		EFFECT->Play("Explosion", WIN_MOUSE_POS);
}

void TileTestScene::Render()
{
	_map->BehindRender();
	ITEMMANAGER->RopeRender();
	_player->Render();
	for (auto monster : _monsters)
		monster->Render();
	ITEMMANAGER->BombRender();
	_map->FrontRender();
}

void TileTestScene::PostRender()
{
	_player->PostRender();
	_map->PostRender();
}

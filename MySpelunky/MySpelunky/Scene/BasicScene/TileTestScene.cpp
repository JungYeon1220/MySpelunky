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

			if (_monsters.size() >= 23)
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
							monster = MONSTER->SetMonster("Spider", Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));
						else if (random == 1)
							monster = MONSTER->SetMonster("Snake", Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));
						else if (random == 2)
							monster = MONSTER->SetMonster("Mosquito", Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));

						_monsters.push_back(monster);
					}

				}
			}
		}
	}

	CAMERA->SetScale(Vector2(0.3f, 0.3f));
	CAMERA->SetPosition(_player->GetCollider()->GetWorldPos() * 0.8f);
	CAMERA->GetViewCollider()->GetTransform()->SetPosition(_player->GetCollider()->GetWorldPos());
	CAMERA->SetTarget(_player->GetCollider()->GetTransform());
	//CAMERA->SetLeftBottom(Vector2(0,0));
	//CAMERA->SetRightTop(Vector2((_map->PoolCount().x - 1) * 100.0f, (_map->PoolCount().y - 1) * 100.0f));
	CAMERA->SetLeftBottom(Vector2(-10000,-10000));
	CAMERA->SetRightTop(Vector2(10000, 10000));

	//SOUND->Play("BGM");
	 
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
		{
			monster->Update();
			monster->IsFalling() = true;
		}
	}
	
	for (auto bomb : ITEMMANAGER->GetBombs())
	{
		bomb->Update();
		bomb->IsFalling() = true;
	}

	for (auto rope : ITEMMANAGER->GetRopes())
		rope->Update();

	_player->IsFalling() = true;
	_player->IsOnOneWay() = false;

	Vector2 playerGrid = MathUtility::GetGridIndex(_player->GetCollider()->GetWorldPos());
	vector<vector<int>> layout = _map->GetLayout();

	for (auto pair : _map->GetTypeTiles())
	{
		for (auto tile : pair.second)
		{
			for (auto monster : _monsters)
			{
				if (monster->IsDead() == true)
					continue;

				Vector2 ObjIdx = MathUtility::GetGridIndex(monster->GetCollider()->GetWorldPos());
				Vector2 tileIdx = tile->GetIndex();
				if (tileIdx.x > ObjIdx.x + 1 || tileIdx.x < ObjIdx.x - 1)
					continue;
				if (tileIdx.y > ObjIdx.y + 1 || tileIdx.y < ObjIdx.y - 1)
					continue;

				if (monster->TileInteract(tile) == true)
					monster->IsFalling() = false;
			}

			for (auto bomb : ITEMMANAGER->GetBombs())
			{
				if (bomb->_boom == true)
				{
					if (bomb->DestroyTile(tile))
					{
						_map->GetLayout()[tile->GetIndex().y][tile->GetIndex().x] = 0;
						_map->_distroyedTileIndex.push_back(tile->GetIndex());
					}
				}

				Vector2 ObjIdx = MathUtility::GetGridIndex(bomb->GetCollider()->GetWorldPos());
				Vector2 tileIdx = tile->GetIndex();
				if (tileIdx.x > ObjIdx.x + 1 || tileIdx.x < ObjIdx.x - 1)
					continue;
				if (tileIdx.y > ObjIdx.y + 1 || tileIdx.y < ObjIdx.y - 1)
					continue;

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

			for (auto rope : ITEMMANAGER->GetRopes())
			{
				if (rope->IsActive() == false || rope->IsHooked() == true)
					continue;

				Vector2 ObjIdx = MathUtility::GetGridIndex(rope->GetCollider()->GetWorldPos());
				Vector2 tileIdx = tile->GetIndex();
				if (tileIdx.x > ObjIdx.x + 1 || tileIdx.x < ObjIdx.x - 1)
					continue;
				if (tileIdx.y > ObjIdx.y + 1 || tileIdx.y < ObjIdx.y - 1)
					continue;

				if (tile->GetType() == Tile::Type::ONE_WAY)
					continue;
				if (tile->Block(rope->GetCollider()))
				{
					rope->GetJumpPower() = 0.0f;
				}
			}

		}
	}

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

		normal->InteractPlayer(_player);
	}
	for (auto unbreakable : _map->GetTypeTiles()["Unbreakable"])
	{
		unbreakable->InteractPlayer(_player);
	}
	for (auto oneway : _map->GetTypeTiles()["Oneway"])
	{
		oneway->InteractPlayer(_player);
	}
	for (auto ladder : _map->GetTypeTiles()["Ladder"])
	{
		ladder->InteractPlayer(_player);
	}
	for (auto wooden : _map->GetTypeTiles()["Wooden"])
	{
		wooden->InteractPlayer(_player);
	}
	for (auto skeleton : _map->GetTypeTiles()["Skeleton"])
	{
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

		skeleton->InteractPlayer(_player);
	}
	for (auto spike : _map->GetTypeTiles()["Spike"])
	{
		if (layout[spike->GetIndex().y + 1][spike->GetIndex().x] == 0)
			spike->IsActive() = false;

		spike->InteractPlayer(_player);
	}
	for (auto movable : _map->GetTypeTiles()["Movable"])
	{
		movable->InteractPlayer(_player);
	}
	_map->_distroyedTileIndex.clear();

	for (auto monster : _monsters)
	{
		if (monster->_isActive == false)
			continue;
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
					_player->TakeDamage(1);
				}
			}
		}
	}

	{
		for (auto bomb : ITEMMANAGER->GetBombs())
		{
			if (bomb->IsActive() == false && bomb->_boom == false)
				continue;

			_player->HoldItem(bomb);

			if (bomb->_boom == true)
			{
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
		}

	}

	{
		for (auto rope : ITEMMANAGER->GetRopes())
		{
			if (rope->IsActive() == false)
				continue;

			float tileX = rope->GetCollider()->GetWorldPos().x;
			float tileY = rope->GetCollider()->GetWorldPos().y;
			float playerX = _player->GetCollider()->GetWorldPos().x;
			float playerY = _player->GetCollider()->GetWorldPos().y;
			float playerXHalfSize = _player->GetSize().x * 0.5f;
			float playerYHalfSize = _player->GetSize().y * 0.5f;

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
					if (i == rope->GetCurLength() - 1)
					{
						if (rope->GetColliders()[i]->GetWorldPos().y >= playerY + playerYHalfSize && _player->IsRope() == true)
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


		item->IsFalling() = true;
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
						item->IsFalling() = false;
					}
				}
			}
		}
	}

	m = true;

	if (KEY_DOWN('P'))
	{
		m = false;
		_player->GetSpeed() = 0.0f;
		_monsters.clear();
		ITEMMANAGER->Init();
		MONSTER->Init();
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
								monster = MONSTER->SetMonster("Spider", Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));
							else if (random == 1)
								monster = MONSTER->SetMonster("Snake", Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));
							else if (random == 2)
								monster = MONSTER->SetMonster("Mosquito", Vector2(j * 100.0f, (_map->PoolCount().y - 1 - i) * 100.0f) + Vector2(0.0f, 100.0f));

							_monsters.push_back(monster);
						}

					}
				}
			}
		}
	}

}

void TileTestScene::Render()
{
	_map->BehindRender();
	ITEMMANAGER->RopeRender();
	_player->Render();
	for (auto monster : _monsters)
		monster->Render();
	ITEMMANAGER->Render();
	_map->FrontRender();

	wstring wstr;
	string str = to_string(_player->_bombCount);
	wstr.assign(str.begin(), str.end());
	FONT->RenderText(L"Bombs : " + wstr, "D2Coding", Vector2(0.0f, 60.0f));

	str = to_string(_player->_ropeCount);
	wstr.assign(str.begin(), str.end());
	FONT->RenderText(L"Ropes : " + wstr, "D2Coding", Vector2(0.0f, 40.0f));

	str = to_string(_player->GetHp());
	wstr.assign(str.begin(), str.end());
	FONT->RenderText(L"HP : " + wstr, "D2Coding", Vector2(0.0f, 20.0f));
}

void TileTestScene::PostRender()
{
	_player->PostRender();
	_map->PostRender();
}

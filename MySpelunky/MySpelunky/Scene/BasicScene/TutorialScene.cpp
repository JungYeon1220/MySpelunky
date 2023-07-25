#include "framework.h"
#include "TutorialScene.h"
#include "../../Object/Obj/Player.h"
#include "../../Object/Obj/Tile/Tile.h"
#include "../../Object/Obj/Map.h"

TutorialScene::TutorialScene()
{
	_map = make_shared<Map>();
	_player = make_shared<Player>();

	_radder1 = make_shared<Tile>();
	_radder1->SetPosition(Vector2(50.0f, 150.0f) - CENTER);
	_radder1->SetType(Tile::Type::LADDER);
	_radder2 = make_shared<Tile>();
	_radder2->SetPosition(Vector2(50.0f, 250.0f) - CENTER);
	_radder2->SetType(Tile::Type::LADDER);

	for (int i = 0; i < 10; i++)
	{
		shared_ptr<Tile> tile = make_shared<Tile>();
		tile->SetPosition(Vector2((i + 0.5f) * 100.0f, 50.0f) - CENTER);
		tile->SetType(Tile::Type::NORMAL);

		_tiles.push_back(tile);
	}

	_block1 = make_shared<Tile>();
	_block2 = make_shared<Tile>();
	_block1->SetType(Tile::Type::NORMAL);
	_block2->SetType(Tile::Type::NORMAL);
	_block1->SetPosition(Vector2(50.0f, 150.0f) - CENTER + Vector2(800.0f, 0.0f));
	_block2->SetPosition(Vector2(50.0f, 150.0f) - CENTER + Vector2(800.0f, 200.0f));

	_oneWay1 = make_shared<Tile>();
	_oneWay1->SetType(Tile::Type::ONE_WAY);
	_oneWay1->SetPosition(Vector2(50.0f, 150.0f) - CENTER + Vector2(400.0f, 0.0f));
	_oneWay2 = make_shared<Tile>();
	_oneWay2->SetType(Tile::Type::ONE_WAY);
	_oneWay2->SetPosition(Vector2(50.0f, 150.0f) - CENTER + Vector2(400.0f,100.0f));

	_tiles.push_back(_block1);
	_tiles.push_back(_block2);
	_tiles.push_back(_radder1);
	_tiles.push_back(_radder2);
	_tiles.push_back(_oneWay1);
	_tiles.push_back(_oneWay2);


	CAMERA->SetTarget(_player->GetCollider()->GetTransform());
	CAMERA->SetLeftBottom(Vector2(-10000.0f, -10000.0f));
	CAMERA->SetRightTop(Vector2(10000.0f, 10000.0f));
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Update()
{
	_player->Update();

	bool check = false;
	bool ladderCheck = false;
	_player->isOnOneWay() = false;

	for (auto tile : _tiles)
	{
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

				if (_player->GetGrabCollider()->IsCollision(tilePos + Vector2(50.0f, 50.0f)) || _player->GetGrabCollider()->IsCollision(tilePos + Vector2(-50.0f, 50.0f)))
				{
					if (_player->GetJumpPower() <= 0.0f)
					{
						if (KEY_PRESS(VK_LEFT) || KEY_PRESS(VK_RIGHT))
						{
							_player->GetCollider()->GetTransform()->SetPosition(Vector2(_player->GetCollider()->GetWorldPos().x, tilePos.y + 20.0f));
							_player->isGrab() = true;
						}
					}
				}
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
					_player->isOnOneWay() = true;

					if (KEY_PRESS(VK_DOWN) && KEY_DOWN('Z'))
					{
						tile->IsActive() = false;
					}
				}

				if (tile->GetCollider()->IsCollision(_player->GetCollider()) == false)
					tile->IsActive() = true;
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
						_player->isClimb() = true;
					}
					ladderCheck = true;
				}
			}
		}
		break;
		default:
			break;
		}

		tile->Update();
	}

	if (check == false)
	{
		_player->IsFalling() = true;
	}
	else
	{
		_player->IsFalling() = false;
		if(KEY_PRESS(VK_DOWN))
			_player->isClimb() = false;
	}

	if(ladderCheck == false)
		_player->isClimb() = false;

	_map->Update();
}

void TutorialScene::Render()
{
	for (auto tile : _tiles)
	{
		if (tile->GetType() == Tile::Type::LADDER || tile->GetType() == Tile::Type::ONE_WAY)
			tile->Render();
	}
	for (int i = 0; i < _map->PoolCount().y; i++)
	{
		for (int j = 0; j < _map->PoolCount().x; j++)
		{
			if (_map->GetTile(j, i)->GetType() == Tile::Type::LADDER || _map->GetTile(j, i)->GetType() == Tile::Type::ONE_WAY)
				_map->GetTile(j, i)->Render();
		}
	}
	_player->Render();
	for (auto tile : _tiles)
	{
		if(tile->GetType() == Tile::Type::NORMAL)
			tile->Render();
	}
	for (int i = 0; i < _map->PoolCount().y; i++)
	{
		for (int j = 0; j < _map->PoolCount().x; j++)
		{
			if (_map->GetTile(j, i)->GetType() == Tile::Type::NORMAL || _map->GetTile(j, i)->GetType() == Tile::Type::UNBREAKABLE)
				_map->GetTile(j, i)->Render();
		}
	}
}

void TutorialScene::PostRender()
{
	_player->PostRender();
	ImGui::Text("is collision : %d", _player->GetGrabCollider()->IsCollision(_block2->GetCollider()->GetWorldPos() + Vector2(50.0f, 50.0f)));
}

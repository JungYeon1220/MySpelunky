#include "framework.h"
#include "TileMapScene.h"
#include "../../Object/Obj/Player.h"
#include "../../Object/Obj/Tile/Tile.h"
#include "../../Object/Obj/Map.h"

TileMapScene::TileMapScene()
{
	_player = make_shared<Player>();
	_map = make_shared<Map>();

	CAMERA->SetTarget(_player->GetCollider()->GetTransform());
	CAMERA->SetLeftBottom(Vector2(-10000.0f, -10000.0f));
	CAMERA->SetRightTop(Vector2(10000.0f, 10000.0f));

}

TileMapScene::~TileMapScene()
{
}

void TileMapScene::Update()
{
	_player->Update();

	bool check = false;
	bool ladderCheck = false;
	_player->isOnOneWay() = false;

	for (auto tiles : _map->GetTiles())
	{
		for (auto tile : tiles)
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
	}

	if (check == false)
	{
		_player->IsFalling() = true;
	}
	else
	{
		_player->IsFalling() = false;
		if (KEY_PRESS(VK_DOWN))
			_player->isClimb() = false;
	}

	if (ladderCheck == false)
		_player->isClimb() = false;
}

void TileMapScene::Render()
{
	//for (auto tiles : _map->GetTiles())
	//{
	//	for (auto tile : tiles)
	//	{
	//		if (tile->GetType() == Tile::Type::LADDER || tile->GetType() == Tile::Type::ONE_WAY)
	//			tile->Render();
	//	}
	//}
	_player->Render();
	//for (auto tiles : _map->GetTiles())
	//{
	//	for (auto tile : tiles)
	//	{
	//		if (tile->GetType() == Tile::Type::NORMAL || tile->GetType() == Tile::Type::UNBREAKABLE)
	//			tile->Render();
	//	}
	//}
	_map->Render();
}

void TileMapScene::PostRender()
{
}

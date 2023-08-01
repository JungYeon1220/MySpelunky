#include "framework.h"
#include "TutorialScene.h"
#include "../../Object/Obj/Player.h"
#include "../../Object/Obj/Tile/Tile.h"
#include "../../Object/Obj/Tile/Normal.h"
#include "../../Object/Obj/Tile/Ladder.h"
#include "../../Object/Obj/Tile/OneWay.h"
#include "../../Object/Obj/Tile/Unbreakable.h"
#include "../../Object/Obj/Tile/Wooden.h"
#include "../../Object/Obj/Tile/Skeleton.h"
#include "../../Object/Obj/Tile/Spike.h"
#include "../../Object/Obj/Tile/Movable.h"
#include "../../Object/Obj/Map.h"

TutorialScene::TutorialScene()
{
	_player = make_shared<Player>();

	for (int i = 0; i < 10; i++)
	{
		shared_ptr<Tile> tile = make_shared<Normal>(Vector2((i + 0.5f) * 100.0f, 50.0f) - CENTER);
		_tiles.push_back(tile);
	}

	_spike = make_shared<Spike>(Vector2(150.0f, 150.0f) - CENTER);
	_tiles.push_back(_spike);
	_movable = make_shared<Movable>(Vector2(250.0f, 350.0f) - CENTER);
	_tiles.push_back(_movable);
	dynamic_pointer_cast<Movable>(_movable)->CanGrab() = true;

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
	_player->IsOnOneWay() = false;

	bool movableCheck = false;

	for (auto tile : _tiles)
	{
		if (tile == nullptr)
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

			if (tile->GetCollider()->Block(_movable->GetCollider()))
				movableCheck = true;
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
				if (dynamic_pointer_cast<Spike>(tile)->GetSpikeCol()->IsCollision(_player->GetCollider()) == true)
				{
					if (_player->GetCollider()->GetWorldPos().y > tile->GetCollider()->GetWorldPos().y)
					{
						if (_player->GetCollider()->GetWorldPos().x >= tile->GetCollider()->GetWorldPos().x - 50.0f && _player->GetCollider()->GetWorldPos().x <= tile->GetCollider()->GetWorldPos().x + 50.0f)
						{
							if (dynamic_pointer_cast<Spike>(tile)->CanSpike() == true)
							{
								_player->Dead();
								dynamic_pointer_cast<Spike>(tile)->SetBlood();
							}

							dynamic_pointer_cast<Spike>(tile)->CanSpike() = false;
						}
					}
				}

				if (tile->GetCollider()->IsCollision(_player->GetCollider()) == false)
					dynamic_pointer_cast<Spike>(tile)->CanSpike() = true;
			}
		}
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
			else
				_player->IsPush() = false;

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
		if (KEY_PRESS(VK_DOWN))
			_player->IsClimb() = false;
	}

	if (movableCheck == false)
	{
		dynamic_pointer_cast<Movable>(_movable)->IsFalling() = true;
	}
	else
	{
		dynamic_pointer_cast<Movable>(_movable)->IsFalling() = false;
	}

	if (ladderCheck == false)
		_player->IsClimb() = false;
}

void TutorialScene::Render()
{
	_player->Render();
	for (auto tile : _tiles)
		tile->Render();
}

void TutorialScene::PostRender()
{

}

#include "framework.h"
#include "Movable.h"

Movable::Movable()
	:Tile()
{
	_movableCol = make_shared<RectCollider>(Vector2(95.0f, 95.0f));
	_movableCol->GetTransform()->SetParent(_col->GetTransform());
	_movableCol->GetTransform()->AddVector2(Vector2(0.0f, -5.0f));
	_name = "Movable";
	_type = Tile::Type::MOVABLE;

}

Movable::Movable(Vector2 pos)
	:Tile(pos)
{
	_movableCol = make_shared<RectCollider>(Vector2(95.0f, 95.0f));
	_movableCol->GetTransform()->SetParent(_col->GetTransform());
	_movableCol->GetTransform()->AddVector2(Vector2(0.0f, -5.0f));
	_name = "Movable";
	_type = Tile::Type::MOVABLE;


}

Movable::~Movable()
{
}

void Movable::Update()
{
	if (_isActive == false)
		return;
	Tile::Update();
	if (_isFalling == true)
		_jumpPower -= GRAVITY * 8;
	else
		_jumpPower = 0.0f;

	if (_jumpPower < -_maxFalling)
		_jumpPower = -_maxFalling;

	if (_isFalling == true)
		_canGrab = false;
	else
		_canGrab = true;

	_col->GetTransform()->AddVector2(Vector2(0.0f, _jumpPower * DELTA_TIME));
	_movableCol->Update();
}

void Movable::Render()
{
	if (_isActive == false)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

	Tile::Render();
	SPRITEMANAGER->Render("Cave", _name);
}

void Movable::InteractPlayer(shared_ptr<class Player> player)
{
	if (_isActive == false)
		return;

	Vector2 playerIdx = MathUtility::GetGridIndex(player->GetCollider()->GetWorldPos());
	Vector2 posIdx = MathUtility::GetGridIndex(_col->GetWorldPos());
	if (posIdx.x > playerIdx.x + 1 || posIdx.x < playerIdx.x - 1)
		return;
	if (posIdx.y > playerIdx.y + 1 || posIdx.y < playerIdx.y - 1)
		return;

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
		if (_isFalling == false)
		{
			if (player->GetCollider()->GetWorldPos().y < _col->GetWorldPos().y + 50.0f
				&& player->GetCollider()->GetWorldPos().y > _col->GetWorldPos().y - 50.0f)
			{
				if (player->IsFalling() == false)
				{
					if (KEY_PRESS(VK_LEFT) && player->GetCollider()->GetWorldPos().x > _col->GetWorldPos().x)
					{
						_col->GetTransform()->AddVector2(-RIGHT_VECTOR * 100.0f * DELTA_TIME);
						player->IsPush() = true;
					}
					else if (KEY_UP(VK_LEFT))
						player->IsPush() = false;

					if (KEY_PRESS(VK_RIGHT) && player->GetCollider()->GetWorldPos().x < _col->GetWorldPos().x)
					{
						_col->GetTransform()->AddVector2(RIGHT_VECTOR * 100.0f * DELTA_TIME);
						player->IsPush() = true;

					}
					else if (KEY_UP(VK_RIGHT))
						player->IsPush() = false;

				}
			}
		}
	}
	else
		player->IsPush() = false;

}

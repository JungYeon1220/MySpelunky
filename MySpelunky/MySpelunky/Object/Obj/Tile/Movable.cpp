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
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

	Tile::Render();
	SPRITEMANAGER->Render("Cave", _name);
}
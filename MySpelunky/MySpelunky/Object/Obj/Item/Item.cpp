#include "framework.h"
#include "Item.h"

Item::Item()
{
	_col = make_shared<RectCollider>(Vector2(50.0f, 50.0f));
	_transform = make_shared<Transform>();
	_transform->SetParent(_col->GetTransform());

	_name = "JumpShoes";
}

Item::~Item()
{
}

void Item::Update()
{
	if (_isFalling == true)
		_jumpPower -= GRAVITY * 8;
	else
		_jumpPower = 0.0f;

	if (_jumpPower < -_maxFalling)
		_jumpPower = -_maxFalling;

	_col->GetTransform()->AddVector2(Vector2(0.0f, _jumpPower * DELTA_TIME));

	_col->Update();
	_transform->Update();
}

void Item::Render()
{
	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Item", _name);

	//_col->Render();
}

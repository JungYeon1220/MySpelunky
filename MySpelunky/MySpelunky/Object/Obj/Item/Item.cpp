#include "framework.h"
#include "Item.h"

Item::Item()
{
	_col = make_shared<RectCollider>(Vector2(50.0f, 50.0f));
	_transform = make_shared<Transform>();
	_transform->SetParent(_col->GetTransform());
}

Item::Item(string name)
	:_name(name)
{
	_col = make_shared<RectCollider>(Vector2(50.0f, 50.0f));
	_transform = make_shared<Transform>();
	_transform->SetParent(_col->GetTransform());
}

Item::~Item()
{
}

void Item::Update()
{
	if (_isActive == false)
		return;

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
	if (_isActive == false)
		return;
	if (_col->IsCollision(CAMERA->GetViewCollider()) == false)
		return;

	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Item", _name);

	//_col->Render();
}

bool Item::UseItem()
{
	return false;
}

void Item::InteractPlayer(shared_ptr<class Player> player)
{
	if (_isActive == false)
		return;

	if (_col->IsCollision(player->GetCollider()))
	{
		_isActive = false;

		if (_name == "JumpShoes")
			player->GetJumpMax() = 1500.0f;
		else if (_name == "Gloves")
			player->_hasGloves = true;
		else if (_name == "BombBox")
			player->_bombCount += 3;
		else if (_name == "Ropes")
			player->_ropeCount += 3;
	}
}

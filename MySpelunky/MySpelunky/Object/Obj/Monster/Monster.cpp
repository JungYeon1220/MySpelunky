#include "framework.h"
#include "Monster.h"
#include "../Player.h"

Monster::Monster()
{
}

Monster::~Monster()
{
}

void Monster::Update()
{
	if (_isDead == true)
		return;

	if (_isFalling == true)
		_jumpPower -= GRAVITY * 8;
	else
		_jumpPower = 0.0f;

	if (_jumpPower < -_maxFalling)
		_jumpPower = -_maxFalling;

	_col->GetTransform()->AddVector2(Vector2(0.0f, _jumpPower * DELTA_TIME));

	_col->GetTransform()->AddVector2(_dir * _curSpeed * DELTA_TIME);

	_col->Update();
	_rangeCol->Update();
	_transform->Update();

}

void Monster::Render()
{
	if (_isDead == true)
		return;

	_transform->SetWorldBuffer(0);
	//_rangeCol->Render();
	//_col->Render();
}

void Monster::SetPosition(Vector2 pos)
{
	_col->GetTransform()->SetPosition(pos);
}

void Monster::Left()
{
}

void Monster::Right()
{
}

void Monster::TakeDamage(int value)
{
	_hp -= value;
	if (_hp <= 0)
	{
		_hp = 0;
		_isDead = true;
	}
}

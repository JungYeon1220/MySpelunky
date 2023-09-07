#include "framework.h"
#include "Bomb.h"

Bomb::Bomb()
{
	_size = Vector2(35.0f, 35.0f);
	_col = make_shared<RectCollider>(_size);
	_offsetTrans = make_shared<Transform>();
	_offsetTrans->SetParent(_col->GetTransform());
	_offsetTrans->SetPosition(Vector2(0.5f, -2.5f));
	_transform = make_shared<Transform>();
	_transform->SetParent(_offsetTrans);
	_transform->SetPosition(Vector2(-0.5f, 2.5f));
	_name = "Bomb";
}

Bomb::~Bomb()
{
}

void Bomb::Update()
{
	if (_isActive == false)
		return;

	if (_isFalling == false && _isActive == true)
	{
		_speed *= 0.9f;
		_rotation = -_speed / 15.0f;
	}

	if (_speed < 0.1f && _speed > -0.1f)
		_speed = 0.0f;

	if (_time >= 3.0f)
	{
		_isActive = false;
		_isFalling = false;
		_time = 0.0f;
		_speed = 0.0f;
		_jumpPower = 0.0f;
	}

	_time += DELTA_TIME;

	_col->GetTransform()->AddVector2(RIGHT_VECTOR * _speed);
	_offsetTrans->AddAngle(_rotation);

	_offsetTrans->Update();
	Item::Update();
}

void Bomb::Rander()
{
	Item::Render();
}

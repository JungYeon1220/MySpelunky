#include "framework.h"
#include "Normal.h"

Normal::Normal()
	:Tile()
{
	_name = "Normal" + to_string(MathUtility::RandomInt(1, 4));
	_type = Tile::Type::NORMAL;
}

Normal::Normal(Vector2 pos)
	: Tile(pos)
{
	_name = "Normal" + to_string(MathUtility::RandomInt(1, 4));
	_type = Tile::Type::NORMAL;
}

Normal::~Normal()
{
}

void Normal::Update()
{
	if (_isActive == false)
		return;
	_col->Update();
	_transform->Update();
}

void Normal::Render()
{
	if (_isActive == false)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

	_transform->SetPosition(Vector2(0, 0));
	_transform->Update();
	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Cave", _name);

	if (_upPebble == true)
	{
		_transform->SetPosition(Vector2(0.0f, 50.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("Cave", "UpPebble" + to_string(_upPebbleNum));
	}
	if (_downPebble == true)
	{
		_transform->SetPosition(Vector2(0.0f, -50.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("Cave", "DownPebble" + to_string(_downPebbleNum));
	}
	if (_rightPebble == true)
	{
		_transform->SetPosition(Vector2(50.0f, 0.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("Cave", "SidePebble" + to_string(_rightPebbleNum));
	}
	if (_leftPebble == true)
	{
		_transform->SetPosition(Vector2(-50.0f, 0.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->GetSprite("Cave")->SetLeft();
		SPRITEMANAGER->Render("Cave", "SidePebble" + to_string(_leftPebbleNum));
		SPRITEMANAGER->GetSprite("Cave")->SetRight();
	}
}

void Normal::Destroy()
{
	_isActive = false;
}

void Normal::SetSpikePebble()
{
	_upPebble = true;
	_upPebbleNum = MathUtility::RandomInt(4, 6);
}

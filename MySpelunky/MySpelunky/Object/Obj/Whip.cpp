#include "framework.h"
#include "Whip.h"

Whip::Whip()
{
	_transform = make_shared<Transform>();
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/char_yellow.png", Vector2(16, 16));
	_col = make_shared<RectCollider>(Vector2(100.0f, 30.0f));
	_col->GetTransform()->SetParent(_transform);

	shared_ptr<SRV> srv = ADD_SRV(L"Resource/Texture/char_yellow.png");
	Vector2 imageSize = srv->GetImageSize();
	Vector2 maxFrame = Vector2(16, 16);
	Vector2 size;
	size.x = imageSize.x / maxFrame.x;
	size.y = imageSize.y / maxFrame.y;

	vector<Action::Clip> clips;
	for (int i = 10; i < 16; i++)
	{
		Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 12.0f / maxFrame.y);
		Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
		clips.push_back(clip);
	}

	_action = make_shared<Action>(clips, "WHIP", Action::END);
	_action->SetEndEvent(std::bind(&Whip::End, this));
}

Whip::~Whip()
{
}

void Whip::Attack()
{
	_isActive = true;
	_action->Reset();
	_action->Play();
}

void Whip::End()
{
	_isActive = false;
}

void Whip::Update()
{
	if (_isActive == false)
		return;

	_action->Update();
	if (_action->GetCurIndex() < 3)
	{
		_transform->SetPosition(_backPos);
		if (_action->GetCurIndex() == 0)
			_col->GetTransform()->SetScale(Vector2(1.0f, 3.0f));
		else
			_col->GetTransform()->SetScale(Vector2(1.0f, 1.5f));
	}
	else if (_action->GetCurIndex() == 3)
	{
		if(_isLeft)
			_transform->SetPosition(_frontPos + Vector2(15.0f, 40.0f));
		else
			_transform->SetPosition(_frontPos + Vector2(-15.0f, 40.0f));
		_col->GetTransform()->SetScale(Vector2(0.7f, 1.2f));
	}
	else
	{
		_transform->SetPosition(_frontPos);
		_col->GetTransform()->SetScale(Vector2(1.0f, 1.0f));

	}

	_transform->Update();
	_sprite->SetCurClip(_action->GetCurClip());
	_sprite->Update();
	_col->Update();
}

void Whip::Render()
{
	if (_isActive == false)
		return;

	_transform->SetWorldBuffer(0);
	_sprite->Render();
	_col->Render();
}

void Whip::SetLeft()
{
	_sprite->SetLeft();
	_frontPos = Vector2(-55.0f, -15.0f);
	_backPos = Vector2(60.0f, 40.0f);
	_isLeft = true;
}

void Whip::SetRight()
{
	_sprite->SetRight();
	_frontPos = Vector2(60.0f, -15.0f);
	_backPos = Vector2(-55.0f, 40.0f);
	_isLeft = false;
}

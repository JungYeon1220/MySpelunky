#include "framework.h"
#include "Unbreakable.h"

Unbreakable::Unbreakable()
	:Tile()
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(rand() % 2, rand() % 5));
	_type = Tile::Type::UNBREAKABLE;

	_upPebbleTrans = make_shared<Transform>();
	_upPebbleTrans->SetParent(_col->GetTransform());
	_upPebbleTrans->SetPosition(Vector2(0.0f, 55.0f));

	_upPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_upPebbleSprite->SetCurClip(Vector2(2 + (rand() % 3), 1));

	_downPebbleTrans = make_shared<Transform>();
	_downPebbleTrans->SetParent(_col->GetTransform());
	_downPebbleTrans->SetPosition(Vector2(0.0f, -55.0f));

	_downPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_downPebbleSprite->SetCurClip(Vector2(2 + (rand() % 3), 2));

	_rightPebbleTrans = make_shared<Transform>();
	_rightPebbleTrans->SetParent(_col->GetTransform());
	_rightPebbleTrans->SetPosition(Vector2(50.0f, 0.0f));

	_rightPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_rightPebbleSprite->SetCurClip(Vector2(2 + (rand() % 3), 0));

	_leftPebbleTrans = make_shared<Transform>();
	_leftPebbleTrans->SetParent(_col->GetTransform());
	_leftPebbleTrans->SetPosition(Vector2(-50.0f, 0.0f));

	_leftPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_leftPebbleSprite->SetCurClip(Vector2(2 + (rand() % 3), 0));
	_leftPebbleSprite->SetLeft();
}

Unbreakable::Unbreakable(Vector2 pos)
	:Tile(pos)
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(rand() % 2, rand() % 5));
	_type = Tile::Type::UNBREAKABLE;

	_upPebbleTrans = make_shared<Transform>();
	_upPebbleTrans->SetParent(_col->GetTransform());
	_upPebbleTrans->SetPosition(Vector2(0.0f, 55.0f));

	_upPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_upPebbleSprite->SetCurClip(Vector2(2 + (rand() % 3), 1));

	_downPebbleTrans = make_shared<Transform>();
	_downPebbleTrans->SetParent(_col->GetTransform());
	_downPebbleTrans->SetPosition(Vector2(0.0f, -55.0f));

	_downPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_downPebbleSprite->SetCurClip(Vector2(2 + (rand() % 3), 2));

	_rightPebbleTrans = make_shared<Transform>();
	_rightPebbleTrans->SetParent(_col->GetTransform());
	_rightPebbleTrans->SetPosition(Vector2(50.0f, 0.0f));

	_rightPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_rightPebbleSprite->SetCurClip(Vector2(2 + (rand() % 3), 0));

	_leftPebbleTrans = make_shared<Transform>();
	_leftPebbleTrans->SetParent(_col->GetTransform());
	_leftPebbleTrans->SetPosition(Vector2(-50.0f, 0.0f));

	_leftPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));
	_leftPebbleSprite->SetCurClip(Vector2(2 + (rand() % 3), 0));
	_leftPebbleSprite->SetLeft();
}

Unbreakable::~Unbreakable()
{
}

void Unbreakable::SetRandomTile(int x, int y)
{
	int m = x % 2;
	int n = y % 2;
	int random = (rand() % 3) * 2;

	_sprite->SetCurClip(Vector2(m, n + random));
}

void Unbreakable::Update()
{
	Tile::Update();
	if (_upPebble == true)
	{
		_upPebbleTrans->Update();
		_upPebbleSprite->Update();
	}
	if (_downPebble == true)
	{
		_downPebbleTrans->Update();
		_downPebbleSprite->Update();
	}
	if (_rightPebble == true)
	{
		_rightPebbleTrans->Update();
		_rightPebbleSprite->Update();
	}
	if (_leftPebble == true)
	{
		_leftPebbleTrans->Update();
		_leftPebbleSprite->Update();
	}
}

void Unbreakable::Render()
{
	Tile::Render();
	if (_upPebble == true)
	{
		_upPebbleTrans->SetWorldBuffer(0);
		_upPebbleSprite->Render();
	}
	if (_downPebble == true)
	{
		_downPebbleTrans->SetWorldBuffer(0);
		_downPebbleSprite->Render();
	}
	if (_rightPebble == true)
	{
		_rightPebbleTrans->SetWorldBuffer(0);
		_rightPebbleSprite->Render();
	}
	if (_leftPebble == true)
	{
		_leftPebbleTrans->SetWorldBuffer(0);
		_leftPebbleSprite->Render();
	}
}

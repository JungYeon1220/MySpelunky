#include "framework.h"
#include "Normal.h"

Normal::Normal()
	:Tile()
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(rand() % 2, rand() % 2));
	_type = Tile::Type::NORMAL;

	_upPebbleTrans = make_shared<Transform>();
	_upPebbleTrans->SetParent(_col->GetTransform());
	_upPebbleTrans->SetPosition(Vector2(0.0f, 50.0f));

	_upPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_upPebbleSprite->SetCurClip(Vector2(5 + (rand() % 3), 6));

	_downPebbleTrans = make_shared<Transform>();
	_downPebbleTrans->SetParent(_col->GetTransform());
	_downPebbleTrans->SetPosition(Vector2(0.0f, -50.0f));

	_downPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_downPebbleSprite->SetCurClip(Vector2(5 + (rand() % 3), 7));

	_rightPebbleTrans = make_shared<Transform>();
	_rightPebbleTrans->SetParent(_col->GetTransform());
	_rightPebbleTrans->SetPosition(Vector2(50.0f, 0.0f));

	_rightPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_rightPebbleSprite->SetCurClip(Vector2(5 + (rand() % 2), 5));

	_leftPebbleTrans = make_shared<Transform>();
	_leftPebbleTrans->SetParent(_col->GetTransform());
	_leftPebbleTrans->SetPosition(Vector2(-50.0f, 0.0f));

	_leftPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_leftPebbleSprite->SetCurClip(Vector2(5 + (rand() % 2), 5));
	_leftPebbleSprite->SetLeft();
}

Normal::Normal(Vector2 pos)
	: Tile(pos)
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(rand() % 2, rand() % 2));
	_type = Tile::Type::NORMAL;

	_upPebbleTrans = make_shared<Transform>();
	_upPebbleTrans->SetParent(_col->GetTransform());
	_upPebbleTrans->SetPosition(Vector2(0.0f, 50.0f));

	_upPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_upPebbleSprite->SetCurClip(Vector2(5 + (rand() % 3), 6));

	_downPebbleTrans = make_shared<Transform>();
	_downPebbleTrans->SetParent(_col->GetTransform());
	_downPebbleTrans->SetPosition(Vector2(0.0f, -50.0f));

	_downPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_downPebbleSprite->SetCurClip(Vector2(5 + (rand() % 3), 7));

	_rightPebbleTrans = make_shared<Transform>();
	_rightPebbleTrans->SetParent(_col->GetTransform());
	_rightPebbleTrans->SetPosition(Vector2(50.0f, 0.0f));

	_rightPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_rightPebbleSprite->SetCurClip(Vector2(5 + (rand() % 2), 5));

	_leftPebbleTrans = make_shared<Transform>();
	_leftPebbleTrans->SetParent(_col->GetTransform());
	_leftPebbleTrans->SetPosition(Vector2(-50.0f, 0.0f));

	_leftPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_leftPebbleSprite->SetCurClip(Vector2(5 + (rand() % 2), 5));
	_leftPebbleSprite->SetLeft();
}

Normal::~Normal()
{
}

void Normal::Update()
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

void Normal::Render()
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

void Normal::SetSpikePebble()
{
	_upPebble = true;
	_upPebbleSprite->SetCurClip(Vector2(5 + rand() % 3, 8));
}

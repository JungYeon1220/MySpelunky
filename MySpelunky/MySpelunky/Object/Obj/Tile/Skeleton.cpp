#include "framework.h"
#include "Skeleton.h"

Skeleton::Skeleton()
	:Tile()
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(10, 2));
	_type = Tile::Type::SKELETON;

	_upPebbleTrans = make_shared<Transform>();
	_upPebbleTrans->SetParent(_col->GetTransform());
	_upPebbleTrans->SetPosition(Vector2(0.0f, 50.0f));

	_upPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_upPebbleSprite->SetCurClip(Vector2(11, 2));
}

Skeleton::Skeleton(Vector2 pos)
	:Tile(pos)
{
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_sprite->SetCurClip(Vector2(10, 2));
	_type = Tile::Type::SKELETON;

	_upPebbleTrans = make_shared<Transform>();
	_upPebbleTrans->SetParent(_col->GetTransform());
	_upPebbleTrans->SetPosition(Vector2(0.0f, 50.0f));

	_upPebbleSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_upPebbleSprite->SetCurClip(Vector2(11, 2));
}

Skeleton::~Skeleton()
{
}

void Skeleton::Update()
{
	Tile::Update();
	if (_upPebble == true)
	{
		_upPebbleTrans->Update();
		_upPebbleSprite->Update();
	}
}

void Skeleton::Render()
{
	Tile::Render();
	if (_upPebble == true)
	{
		_upPebbleTrans->SetWorldBuffer(0);
		_upPebbleSprite->Render();
	}
}

void Skeleton::SetSpikePebble()
{
	_upPebble = true;
	_upPebbleSprite->SetCurClip(Vector2(8, 8));
}

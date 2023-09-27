#include "framework.h"
#include "SpriteManager.h"

SpriteManager* SpriteManager::_instance = nullptr;

SpriteManager::SpriteManager()
{
	CreateSprite();
}

SpriteManager::~SpriteManager()
{
}

void SpriteManager::Render(string spriteName, string frameName)
{
	_sprites[spriteName].sprite->SetCurClip(_sprites[spriteName].frame[frameName]);
	_sprites[spriteName].sprite->Update();
	_sprites[spriteName].sprite->Render();
}

void SpriteManager::Render(string spriteName, Action::Clip clip)
{
	_sprites[spriteName].sprite->SetCurClip(clip);
	_sprites[spriteName].sprite->Update();
	_sprites[spriteName].sprite->Render();
}

void SpriteManager::CreateSprite()
{
	{
		SpriteData data;

		data.sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));

		data.frame["Normal1"] = Vector2(0, 0);
		data.frame["Normal2"] = Vector2(1, 0);
		data.frame["Normal3"] = Vector2(0, 1);
		data.frame["Normal4"] = Vector2(1, 1);

		data.frame["UpPebble1"] = Vector2(5, 6);
		data.frame["UpPebble2"] = Vector2(6, 6);
		data.frame["UpPebble3"] = Vector2(7, 6);

		// SpikePebble
		data.frame["UpPebble4"] = Vector2(5, 8);
		data.frame["UpPebble5"] = Vector2(6, 8);
		data.frame["UpPebble6"] = Vector2(7, 8);

		data.frame["DownPebble1"] = Vector2(5, 7);
		data.frame["DownPebble2"] = Vector2(6, 7);
		data.frame["DownPebble3"] = Vector2(7, 7);

		data.frame["SidePebble1"] = Vector2(5, 5);
		data.frame["SidePebble2"] = Vector2(6, 5);

		// GrabPebble
		data.frame["SidePebble3"] = Vector2(7, 5);

		data.frame["NormalLadder"] = Vector2(4, 1);
		data.frame["TopLadder"] = Vector2(4, 0);
		data.frame["BottomLadder"] = Vector2(4, 3);
		data.frame["PlatformLadder"] = Vector2(4, 2);

		data.frame["Movable"] = Vector2(7, 0);

		data.frame["Skeleton"] = Vector2(10, 2);
		data.frame["SkeletonPebble"] = Vector2(11, 2);
		data.frame["SkeletonSpikePebble"] = Vector2(8, 8);

		data.frame["Spike1"] = Vector2(5, 9);
		data.frame["Spike2"] = Vector2(6, 9);
		data.frame["Spike3"] = Vector2(7, 9);

		data.frame["Blood1"] = Vector2(5, 10);
		data.frame["Blood2"] = Vector2(6, 10);
		data.frame["Blood3"] = Vector2(7, 10);

		data.frame["SkeletonSpike"] = Vector2(8, 9);
		data.frame["SkeletonBlood"] = Vector2(5, 10);

		_sprites["Cave"] = data;
	}

	{
		SpriteData data;

		data.sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floormisc.png", Vector2(8, 8), Vector2(100.0f, 100.0f));

		data.frame["Oneway"] = Vector2(1, 1);
		data.frame["OnewayLeg"] = Vector2(1, 2);
		data.frame["OnewayBottom"] = Vector2(1, 3);

		_sprites["FloorMisc"] = data;
	}

	{
		SpriteData data;

		data.sprite = make_shared<Sprite_Frame>(L"Resource/Texture/border_main.png", Vector2(8, 8), Vector2(100.0f, 100.0f));

		data.frame["Unbreakable11"] = Vector2(0, 0);
		data.frame["Unbreakable12"] = Vector2(1, 0);
		data.frame["Unbreakable13"] = Vector2(0, 1);
		data.frame["Unbreakable14"] = Vector2(1, 1);

		data.frame["Unbreakable21"] = Vector2(0, 0 + 2);
		data.frame["Unbreakable22"] = Vector2(1, 0 + 2);
		data.frame["Unbreakable23"] = Vector2(0, 1 + 2);
		data.frame["Unbreakable24"] = Vector2(1, 1 + 2);

		data.frame["Unbreakable31"] = Vector2(0, 0 + 4);
		data.frame["Unbreakable32"] = Vector2(1, 0 + 4);
		data.frame["Unbreakable33"] = Vector2(0, 1 + 4);
		data.frame["Unbreakable34"] = Vector2(1, 1 + 4);

		data.frame["UBUpPebble1"] = Vector2(2, 1);
		data.frame["UBUpPebble2"] = Vector2(3, 1);
		data.frame["UBUpPebble3"] = Vector2(4, 1);

		data.frame["UBDownPebble1"] = Vector2(2, 2);
		data.frame["UBDownPebble2"] = Vector2(3, 2);
		data.frame["UBDownPebble3"] = Vector2(4, 2);

		data.frame["UBSidePebble1"] = Vector2(2, 0);
		data.frame["UBSidePebble2"] = Vector2(3, 0);
		data.frame["UBSidePebble3"] = Vector2(4, 0);

		_sprites["BorderMain"] = data;
	}

	{
		SpriteData data;

		data.sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floorstyled_wood.png", Vector2(10, 10), Vector2(100.0f, 100.0f));

		data.frame["Wooden"] = Vector2(7, 2);

		_sprites["Wood"] = data;
	}

	{
		SpriteData data;

		data.sprite = make_shared<Sprite_Frame>(L"Resource/Texture/items.png", Vector2(16, 16), Vector2(70.0f, 70.0f));

		data.frame["JumpShoes"] = Vector2(7, 2);
		data.frame["Stone"] = Vector2(0, 1);
		data.frame["Bomb"] = Vector2(0, 5);
		data.frame["BombBox"] = Vector2(1, 2);
		data.frame["Gloves"] = Vector2(4, 2);
		data.frame["Ropes"] = Vector2(0, 6);
		data.frame["Chicken"] = Vector2(15, 14);

		_sprites["Item"] = data;
	}

	{
		SpriteData data;

		data.sprite = make_shared<Sprite_Frame>(L"Resource/Texture/mosquito.png", Vector2(3, 3), Vector2(70.0f, 70.0f));

		_sprites["Mosquito"] = data;
	}

	{
		SpriteData data;

		data.sprite = make_shared<Sprite_Frame>(L"Resource/Texture/snake.png", Vector2(4, 3), Vector2(70.0f, 70.0f));

		_sprites["Snake"] = data;
	}

	{
		SpriteData data;

		data.sprite = make_shared<Sprite_Frame>(L"Resource/Texture/spider.png", Vector2(4, 4), Vector2(50.0f, 50.0f));

		_sprites["Spider"] = data;
	}

	{
		SpriteData data;

		data.sprite = make_shared<Sprite_Frame>(L"Resource/Texture/char_yellow.png", Vector2(16, 16), Vector2(100.0f, 100.0f));

		data.frame["Hook1"] = Vector2(12, 9);
		data.frame["Hook2"] = Vector2(13, 9);
		data.frame["Hook3"] = Vector2(14, 9);

		data.frame["Rope"] = Vector2(0, 12);
		data.frame["RopeEnd"] = Vector2(5, 12);

		_sprites["Rope"] = data;
	}
}

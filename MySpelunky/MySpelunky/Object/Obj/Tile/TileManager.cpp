#include "framework.h"
#include "TileManager.h"

TileManager* TileManager::_instance = nullptr;

TileManager::TileManager()
{
	CreateSprite();
}

TileManager::~TileManager()
{
}

void TileManager::CreateSprite()
{
	_caveSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
	_caveFramePos["Nomal"] = Vector2(0, 0);
}

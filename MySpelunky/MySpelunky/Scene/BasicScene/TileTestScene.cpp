#include "framework.h"
#include "TileTestScene.h"
#include "../../Object/Obj/Tile/Normal.h"
#include "../../Object/Obj/Tile/Ladder.h"
#include "../../Object/Obj/Tile/OneWay.h"
#include "../../Object/Obj/Tile/Unbreakable.h"
#include "../../Object/Obj/Map.h"

TileTestScene::TileTestScene()
{
	_map = make_shared<Map>();
	CAMERA->SetScale(Vector2(0.3f, 0.3f));
}

TileTestScene::~TileTestScene()
{
}

void TileTestScene::Update()
{
	_map->Update();
}

void TileTestScene::Render()
{
	_map->Render();
}

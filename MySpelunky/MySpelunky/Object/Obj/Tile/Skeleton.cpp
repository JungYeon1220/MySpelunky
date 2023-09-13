#include "framework.h"
#include "Skeleton.h"

Skeleton::Skeleton()
	:Tile()
{
	_name = "Skeleton";
	_type = Tile::Type::SKELETON;
}

Skeleton::Skeleton(Vector2 pos)
	:Tile(pos)
{
	_name = "Skeleton";
	_type = Tile::Type::SKELETON;
}

Skeleton::~Skeleton()
{
}

void Skeleton::Update()
{
	if (_isActive == false)
		return;
	_col->Update();
}

void Skeleton::Render()
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
		SPRITEMANAGER->Render("Cave", _pebbleName);
	}
}

void Skeleton::SetSpikePebble()
{
	_upPebble = true;
	_pebbleName = "SkeletonSpikePebble";
}

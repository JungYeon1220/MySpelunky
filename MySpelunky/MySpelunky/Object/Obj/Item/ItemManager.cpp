#include "framework.h"
#include "ItemManager.h"
#include "Item.h"
#include "Bomb.h"
#include "Rope.h"

ItemManager* ItemManager::_instance = nullptr;

ItemManager::ItemManager()
{
	for (int i = 0; i < 10; i++)
	{
		shared_ptr<Bomb> bomb = make_shared<Bomb>();
		bomb->GetCollider()->GetTransform()->SetPosition(Vector2(-10000.0f, 10000.0f));
		_bombs.push_back(bomb);
	}
	for (int i = 0; i < 10; i++)
	{
		shared_ptr<Rope> rope = make_shared<Rope>();
		rope->GetCollider()->GetTransform()->SetPosition(Vector2(-10000.0f, 10000.0f));
		_ropes.push_back(rope);
	}
}

ItemManager::~ItemManager()
{
}

void ItemManager::Update()
{
	for (auto rope : _ropes)
		rope->Update();
	for (auto bomb : _bombs)
		bomb->Update();
	for (auto item : _items)
		item->Update();
}

void ItemManager::BombRender()
{
	for (auto item : _items)
		item->Render();
	for (auto bomb : _bombs)
		bomb->Render();
}

void ItemManager::RopeRender()
{
	for (auto rope : _ropes)
		rope->Render();
}

void ItemManager::SetItem(string name, Vector2 pos)
{
	shared_ptr<Item> item = make_shared<Item>();
	item->SetName(name);
	item->GetCollider()->GetTransform()->SetPosition(pos);
	_items.push_back(item);
}

bool ItemManager::ThrowBomb(Vector2 pos, float speedX, float speedY)
{
	shared_ptr<Bomb> bomb = nullptr;
	for (auto finder : _bombs)
	{
		if (finder->IsActive() == false)
		{
			bomb = finder;
			break;
		}
	}

	if (bomb == nullptr)
		return false;

	bomb->IsFalling() = true;
	bomb->IsActive() = true;
	bomb->GetCollider()->GetTransform()->SetPosition(pos);

	bomb->GetSpeed() = speedX;
	bomb->GetJumpPower() = speedY;
	bomb->GetRotation() = MathUtility::RandomFloat(-0.3f, 0.3f);
	return true;
}

void ItemManager::ThrowRope(Vector2 pos)
{
	shared_ptr<Rope> rope = nullptr;
	for (auto finder : _ropes)
	{
		if (finder->IsActive() == false)
		{
			rope = finder;
			break;
		}
	}
	if (rope == nullptr)
		return;

	rope->GetCollider()->GetTransform()->SetPosition({ MathUtility::GetGridPosition(pos).x, pos.y });
	rope->IsActive() = true;
}

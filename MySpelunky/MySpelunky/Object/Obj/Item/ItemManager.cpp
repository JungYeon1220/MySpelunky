#include "framework.h"
#include "ItemManager.h"

ItemManager* ItemManager::_instance = nullptr;

ItemManager::ItemManager()
{
	for (int i = 0; i < 10; i++)
	{
		shared_ptr<Bomb> bomb = make_shared<Bomb>();
		bomb->GetCollider()->GetTransform()->SetPosition(Vector2(-10000.0f, -10000.0f));
		_bombs.push_back(bomb);
	}
	for (int i = 0; i < 10; i++)
	{
		shared_ptr<Rope> rope = make_shared<Rope>();
		rope->GetCollider()->GetTransform()->SetPosition(Vector2(-10000.0f, -10000.0f));
		_ropes.push_back(rope);
	}
}

ItemManager::~ItemManager()
{
}

void ItemManager::Update()
{
	for (auto item : _items)
		item->Update();
}

void ItemManager::Render()
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

void ItemManager::Init()
{
	for (auto bomb : _bombs)
		bomb->Reset();
	for (auto rope : _ropes)
		rope->Reset();
	_items.clear();
}

void ItemManager::SetItem(string name, Vector2 pos)
{
	shared_ptr<Item> item = make_shared<Item>(name);
	item->GetCollider()->GetTransform()->SetPosition(pos);
	item->IsActive() = true;
	_items.push_back(item);

}

void ItemManager::SetRandomItem(Vector2 pos)
{
	int temp = MathUtility::RandomInt(0, 4);
	string str;
	switch (temp)
	{
	case 0:
		str = "JumpShoes";
		break;
	case 1:
		str = "Gloves";
		break;
	case 2:
		str = "BombBox";
		break;
	case 3:
		str = "Ropes";
		break;
	case 4:
		str = "Chicken";
		break;
	default:
		break;
	}

	SetItem(str, pos);
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
	bomb->_boom = false;
	bomb->GetCollider()->GetTransform()->SetPosition(pos);

	bomb->GetSpeed() = speedX;
	bomb->GetJumpPower() = speedY;
	bomb->GetRotation() = MathUtility::RandomFloat(-0.3f, 0.3f);
	return true;
}

void ItemManager::ThrowRope(Vector2 pos, bool laying, bool left)
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

	if (laying == true)
	{
		Vector2 ropePos = MathUtility::GetGridPosition(pos);
		rope->GetJumpPower() = 0.0f;
		if (left == false)
			rope->GetCollider()->GetTransform()->SetPosition(ropePos + RIGHT_VECTOR * 100.0f);
		else
			rope->GetCollider()->GetTransform()->SetPosition(ropePos - RIGHT_VECTOR * 100.0f);
	}
	else
		rope->GetCollider()->GetTransform()->SetPosition({ MathUtility::GetGridPosition(pos).x, pos.y });

	rope->IsActive() = true;
}

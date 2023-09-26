#include "framework.h"
#include "MonsterManager.h"

MonsterManager* MonsterManager::_instance = nullptr;
MonsterManager::MonsterManager()
{
	for (int i = 0; i < _poolCount; i++)
	{
		shared_ptr<Monster> monster = make_shared<Snake>();
		monster->SetPosition(Vector2(-1000, 1000));
		monster->Deactive();
		_monsters["Snake"].push_back(monster);
	}

	for (int i = 0; i < _poolCount; i++)
	{
		shared_ptr<Monster> monster = make_shared<Spider>();
		monster->SetPosition(Vector2(-1000, 1000));
		monster->Deactive();
		_monsters["Spider"].push_back(monster);
	}

	for (int i = 0; i < _poolCount; i++)
	{
		shared_ptr<Monster> monster = make_shared<Mosquito>();
		monster->SetPosition(Vector2(-1000, 1000));
		monster->Deactive();
		_monsters["Mosquito"].push_back(monster);
	}
}

MonsterManager::~MonsterManager()
{
}

shared_ptr<Monster> MonsterManager::SetMonster(string name, Vector2 pos)
{
	if (_monsters.count(name) == 0)
		return nullptr;

	for (auto monster : _monsters[name])
	{
		if (monster->_isActive == false)
		{
			monster->SetPosition(pos);
			monster->_isActive = true;
			return monster;
		}
	}
}

void MonsterManager::Update()
{
	for (auto pair : _monsters)
	{
		for (auto monster : pair.second)
		{
			monster->Update();
			monster->IsFalling() = true;
		}
	}
}

void MonsterManager::Render()
{
	for (auto pair : _monsters)
	{
		for (auto monster : pair.second)
		{
			monster->Render();
		}
	}
}

void MonsterManager::Init()
{
	for (auto pair : _monsters)
	{
		for (auto monster : pair.second)
		{
			monster->SetPosition(Vector2(-1000, 1000));
			monster->Deactive();
		}
	}
}

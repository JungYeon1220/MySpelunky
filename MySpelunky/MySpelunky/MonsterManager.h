#pragma once
class MonsterManager
{
private:
	MonsterManager();
	~MonsterManager();

public:
	static void Create()
	{
		if (_instance == nullptr)
			_instance = new MonsterManager();
	}

	static void Delete()
	{
		if (_instance != nullptr)
			delete _instance;
	}

	static MonsterManager* GetInstance()
	{
		if (_instance != nullptr)
			return _instance;

		return nullptr;
	}

	void SetMonster(Vector2 pos, string name = "");

private:
	static MonsterManager* _instance;

	vector<Monster> _monsters;
};


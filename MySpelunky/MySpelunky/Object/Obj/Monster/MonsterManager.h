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

	shared_ptr<Monster> SetMonster(string name, Vector2 pos);
	void Update();
	void Render();
	void Init();

	map<string, vector<shared_ptr<Monster>>> GetMonsters() { return _monsters; }

private:
	static MonsterManager* _instance;

	int _poolCount = 20;

	map<string, vector<shared_ptr<Monster>>> _monsters;
};


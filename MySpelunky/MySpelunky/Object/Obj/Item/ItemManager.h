#pragma once
class ItemManager
{
private:
	ItemManager();
	~ItemManager();

public:
	static void Create()
	{
		if (_instance == nullptr)
			_instance = new ItemManager();
	}

	static void Delete()
	{
		if (_instance != nullptr)
			delete _instance;
	}

	static ItemManager* GetInstance()
	{
		if (_instance != nullptr)
			return _instance;

		return nullptr;
	}

	void Update();
	void Render();

	bool ThrowBomb(Vector2 pos, float speedX, float speedY);
	void ThrowRope(Vector2 pos);


	vector<shared_ptr<class Bomb>> GetBombs() { return _bombs; }
	vector<shared_ptr<class Rope>> GetRopes() { return _ropes; }

private:
	static ItemManager* _instance;

	vector<shared_ptr<class Bomb>> _bombs;
	vector<shared_ptr<class Rope>> _ropes;

};


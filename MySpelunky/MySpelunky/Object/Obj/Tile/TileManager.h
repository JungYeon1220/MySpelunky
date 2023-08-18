#pragma once
class TileManager
{
private:
	TileManager();
	~TileManager();

public:
	static void Create()
	{
		if (_instance == nullptr)
			_instance = new TileManager();
	}

	static void Delete()
	{
		if (_instance != nullptr)
			delete _instance;
	}

	static TileManager* GetInstance()
	{
		if (_instance != nullptr)
			return _instance;

		return nullptr;
	}

private:
	void CreateSprite();
	static TileManager* _instance;

	unordered_map<string, Vector2> _caveFramePos;
	shared_ptr<Sprite_Frame> _caveSprite;
};


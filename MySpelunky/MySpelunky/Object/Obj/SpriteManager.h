#pragma once
class SpriteManager
{
private:
	SpriteManager();
	~SpriteManager();

public:
	static void Create()
	{
		if (_instance == nullptr)
			_instance = new SpriteManager();
	}

	static void Delete()
	{
		if (_instance != nullptr)
			delete _instance;
	}

	static SpriteManager* GetInstance()
	{
		if (_instance != nullptr)
			return _instance;

		return nullptr;
	}
	struct SpriteData
	{
		shared_ptr<Sprite_Frame> sprite;
		map<string, Vector2> frame;
	};

	void Render(string spriteName, string frameName);
	void Render(string spriteName, Action::Clip clip);

	shared_ptr<Sprite_Frame> GetSprite(string name) { return _sprites[name].sprite; }
private:
	void CreateSprite();
	static SpriteManager* _instance;

	map<string, SpriteData> _sprites;
};


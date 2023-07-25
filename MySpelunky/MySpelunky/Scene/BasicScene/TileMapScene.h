#pragma once
class TileMapScene : public Scene
{
public:
	TileMapScene();
	~TileMapScene();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
private:
	shared_ptr<class Map> _map;
	shared_ptr<class Player> _player;
};


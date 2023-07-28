#pragma once
class TileTestScene : public Scene
{
public:
	TileTestScene();
	~TileTestScene();

	virtual void Update() override;
	virtual void Render() override;
private:
	shared_ptr<Tile> _normal;
	shared_ptr<Tile> _normal2;
	shared_ptr<Tile> _ladder;
	shared_ptr<Tile> _oneWay;
	shared_ptr<Tile> _unbreakable;
	shared_ptr<Tile> _unbreakable2;
	shared_ptr<class Map> _map;
	shared_ptr<class Player> _player;
};


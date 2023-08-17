#pragma once
class TileTestScene : public Scene
{
public:
	TileTestScene();
	~TileTestScene();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
private:
	shared_ptr<class Map> _map;
	shared_ptr<class Player> _player;
	vector<shared_ptr<Monster>> _monsters;
};


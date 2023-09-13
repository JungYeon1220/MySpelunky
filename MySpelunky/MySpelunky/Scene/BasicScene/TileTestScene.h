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
	shared_ptr<Map> _map;
	shared_ptr<Player> _player;
	vector<shared_ptr<Monster>> _monsters;

	bool m = false;
};


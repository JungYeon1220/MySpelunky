#pragma once
class TutorialScene : public Scene
{
public:
	TutorialScene();
	virtual ~TutorialScene();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	shared_ptr<class Map> _map;
	shared_ptr<class Player> _player;
	shared_ptr<class Tile> _radder1;
	shared_ptr<class Tile> _radder2;
	shared_ptr<class Tile> _block1;
	shared_ptr<class Tile> _block2;
	shared_ptr<class Tile> _oneWay1;
	shared_ptr<class Tile> _oneWay2;
	vector<shared_ptr<class Tile>> _tiles;
};


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
	shared_ptr<class Player> _player;
	vector<shared_ptr<Monster>> _monsters;
	shared_ptr<Monster> _spider;
	shared_ptr<Monster> _snake;
	shared_ptr<Monster> _mosquito;
	vector<shared_ptr<class Tile>> _tiles;
	shared_ptr<class Tile> _spike;
	shared_ptr<class Tile> _movable;
	shared_ptr<Item> _item;
};


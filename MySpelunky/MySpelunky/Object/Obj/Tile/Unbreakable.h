#pragma once
class Unbreakable : public Tile
{
public:
	Unbreakable();
	Unbreakable(Vector2 pos);
	virtual ~Unbreakable();

	void SetRandomTile(int x, int y);

	void PebbleUp() { _upPebble = true; }
	void PebbleDown() { _downPebble = true; }
	void PebbleRight() { _rightPebble = true; }
	void PebbleLeft() { _leftPebble = true; }

	virtual void Update() override;
	virtual void Render() override;

private:
	shared_ptr<Transform> _upPebbleTrans;
	shared_ptr<Sprite_Frame> _upPebbleSprite;
	bool _upPebble = false;

	shared_ptr<Transform> _downPebbleTrans;
	shared_ptr<Sprite_Frame> _downPebbleSprite;
	bool _downPebble = false;

	shared_ptr<Transform> _rightPebbleTrans;
	shared_ptr<Sprite_Frame> _rightPebbleSprite;
	bool _rightPebble = false;

	shared_ptr<Transform> _leftPebbleTrans;
	shared_ptr<Sprite_Frame> _leftPebbleSprite;
	bool _leftPebble = false;
};


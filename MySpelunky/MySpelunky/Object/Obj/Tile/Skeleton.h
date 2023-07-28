#pragma once
class Skeleton : public Tile
{
public:
	Skeleton();
	Skeleton(Vector2 pos);
	~Skeleton();

	virtual void Update() override;
	virtual void Render() override;

	void PebbleUp() { _upPebble = true; }
	void SetSpikePebble();

	bool& CanGrab() { return _canGrab; }

private:
	bool _canGrab = true;
	shared_ptr<Transform> _upPebbleTrans;
	shared_ptr<Sprite_Frame> _upPebbleSprite;
	bool _upPebble = false;
};


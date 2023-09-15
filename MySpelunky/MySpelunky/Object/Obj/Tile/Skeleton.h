#pragma once
class Skeleton : public Tile
{
public:
	Skeleton();
	Skeleton(Vector2 pos);
	virtual ~Skeleton();

	virtual void Update() override;
	virtual void Render() override;

	virtual void InteractPlayer(shared_ptr<class Player> player);
	virtual void Destroy() { _isActive = false; }

	virtual void PebbleUp() { _upPebble = true; }
	void SetSpikePebble();

private:
	string _pebbleName = "SkeletonPebble";
	bool _upPebble = false;
};


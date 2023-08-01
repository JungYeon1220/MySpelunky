#pragma once
class Spike : public Tile
{
public:
	Spike();
	Spike(Vector2 pos);
	virtual ~Spike();

	virtual bool Block(shared_ptr<Collider> col) override;

	virtual void Update() override;
	virtual void Render() override;

	void SetSkeletonSpike();
	void SetBlood() { _isBlood = true; }

	shared_ptr<RectCollider> GetSpikeCol() { return _spikeCol; }

	bool& CanSpike() { return _CanSpike; }
private:
	shared_ptr<Sprite_Frame> _bloodSprite;
	shared_ptr<RectCollider> _spikeCol;
	bool _isBlood = false;
	bool _isSkeleton = false;
	bool _CanSpike = true;

};


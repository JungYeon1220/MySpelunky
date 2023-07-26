#pragma once
class Spike : public Tile
{
public:
	Spike();
	Spike(Vector2 pos);
	~Spike();

	virtual bool Block(shared_ptr<Collider> col) override;

	virtual void Update() override;
	virtual void Render() override;

	void SetSkeletonSpike();
	void SetBlood() { _isBlood = true; }

private:
	shared_ptr<Sprite_Frame> _bloodSprite;
	bool _isBlood = false;
	bool _isSkeleton = false;

};


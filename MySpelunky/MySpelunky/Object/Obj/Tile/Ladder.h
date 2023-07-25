#pragma once
class Ladder : public Tile
{
public:
	Ladder();
	Ladder(Vector2 pos);
	~Ladder();

	virtual bool Block(shared_ptr<Collider> col) override;

	virtual void Update() override;
	virtual void Render() override;

	void SetTop();
	void SetMiddle();
	void SetBottom();
	void SetOneWay();

	bool& CanStand() { return _canStand; }
private:
	bool _isOneWay = false;
	bool _canStand = true;
};


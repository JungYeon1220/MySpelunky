#pragma once
class Ladder : public Tile
{
public:
	Ladder();
	Ladder(Vector2 pos);
	virtual ~Ladder();

	virtual bool Block(shared_ptr<Collider> col) override;

	virtual void Update() override;
	virtual void Render() override;

	void SetTop();
	void SetMiddle();
	void SetBottom();
	void SetOneWay();

	bool& CanStand() { return _canStand; }
	bool IsOneWay() { return _isOneWay; }
private:
	bool _isOneWay = false;
	bool _canStand = true;
};


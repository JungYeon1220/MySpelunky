#pragma once
class OneWay : public Tile
{
public:
	OneWay();
	OneWay(Vector2 pos);
	~OneWay();

	bool Block(shared_ptr<Collider> col) override;

	virtual void Update() override;
	virtual void Render() override;

	bool& CanStand() { return _canStand; }

private:
	bool _canStand = true;
};


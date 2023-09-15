#pragma once
class OneWay : public Tile
{
public:
	OneWay();
	OneWay(Vector2 pos);
	virtual ~OneWay();

	bool Block(shared_ptr<Collider> col) override;
	virtual void InteractPlayer(shared_ptr<class Player> player);

	virtual void Update() override;
	virtual void Render() override;

	bool& CanStand() { return _canStand; }
	int& LegCount() { return _legCount; }
private:
	bool _canStand = true;

	int _legCount = 0;
};


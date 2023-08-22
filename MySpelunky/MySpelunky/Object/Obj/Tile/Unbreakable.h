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
	int _upPebbleNum = MathUtility::RandomInt(1, 3);
	bool _upPebble = false;

	int _downPebbleNum = MathUtility::RandomInt(1, 3);
	bool _downPebble = false;

	int _rightPebbleNum = MathUtility::RandomInt(1, 3);
	bool _rightPebble = false;

	int _leftPebbleNum = MathUtility::RandomInt(1, 3);
	bool _leftPebble = false;
};


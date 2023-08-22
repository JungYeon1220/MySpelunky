#pragma once
class Normal : public Tile
{
public:
	Normal();
	Normal(Vector2 pos);
	virtual ~Normal();

	virtual void Update() override;
	virtual void Render() override;

	void PebbleUp() { _upPebble = true; }
	void PebbleDown() { _downPebble = true; }
	void PebbleRight() { _rightPebble = true; }
	void PebbleGrabRight() { _rightPebbleNum = 3; }
	void PebbleLeft() { _leftPebble = true; }
	void PebbleGrabLeft() { _rightPebbleNum = 3; }

	void SetSpikePebble();

private:
	int _upPebbleNum = MathUtility::RandomInt(1, 3);
	shared_ptr<Transform> _upPebbleTrans;
	bool _upPebble = false;

	int _downPebbleNum = MathUtility::RandomInt(1, 3);
	shared_ptr<Transform> _downPebbleTrans;
	bool _downPebble = false;

	int _rightPebbleNum = MathUtility::RandomInt(1, 2);
	shared_ptr<Transform> _rightPebbleTrans;
	bool _rightPebble = false;

	int _leftPebbleNum = MathUtility::RandomInt(1, 2);
	shared_ptr<Transform> _leftPebbleTrans;
	bool _leftPebble = false;
};


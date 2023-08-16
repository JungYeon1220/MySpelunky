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
	void PebbleGrabRight() { _rightPebbleSprite->SetCurClip(Vector2(7, 5)); }
	void PebbleLeft() { _leftPebble = true; }
	void PebbleGrabLeft() { _leftPebbleSprite->SetCurClip(Vector2(7, 5)); }

	void SetSpikePebble();


private:


	shared_ptr<Transform> _upPebbleTrans;
	shared_ptr<Sprite_Frame> _upPebbleSprite;
	bool _upPebble = false;

	shared_ptr<Transform> _downPebbleTrans;
	shared_ptr<Sprite_Frame> _downPebbleSprite;
	bool _downPebble = false;

	shared_ptr<Transform> _rightPebbleTrans;
	shared_ptr<Sprite_Frame> _rightPebbleSprite;
	bool _rightPebble = false;

	shared_ptr<Transform> _leftPebbleTrans;
	shared_ptr<Sprite_Frame> _leftPebbleSprite;
	bool _leftPebble = false;
};


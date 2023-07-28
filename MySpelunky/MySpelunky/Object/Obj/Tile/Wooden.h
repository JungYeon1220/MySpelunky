#pragma once
class Wooden : public Tile
{
public:
	Wooden();
	Wooden(Vector2 pos);
	~Wooden();

	virtual void Update() override;
	virtual void Render() override;

	bool& CanGrab() { return _canGrab; }
private:
	bool _canGrab = true;
};


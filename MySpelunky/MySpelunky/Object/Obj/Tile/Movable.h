#pragma once
class Movable : public Tile
{
public:
	Movable();
	Movable(Vector2 pos);
	~Movable();

	virtual void Update() override;
	virtual void Render() override;

private:
};


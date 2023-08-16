#pragma once
class Wooden : public Tile
{
public:
	Wooden();
	Wooden(Vector2 pos);
	virtual ~Wooden();

	virtual void Update() override;
	virtual void Render() override;

private:
};


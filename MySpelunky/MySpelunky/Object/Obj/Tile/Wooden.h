#pragma once
class Wooden : public Tile
{
public:
	Wooden();
	Wooden(Vector2 pos);
	~Wooden();

	virtual void Update() override;
	virtual void Render() override;

private:
	
};


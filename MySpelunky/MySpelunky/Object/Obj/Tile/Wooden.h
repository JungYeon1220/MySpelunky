#pragma once
class Wooden : public Tile
{
public:
	Wooden();
	Wooden(Vector2 pos);
	virtual ~Wooden();

	virtual void Destroy() { _isActive = false; }

	virtual void Update() override;
	virtual void Render() override;

private:
};


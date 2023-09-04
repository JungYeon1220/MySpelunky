#pragma once
class Rope : public Item
{
public:
	Rope();
	virtual ~Rope();

	void Update();
	void Render();

private:
	int _length = 4;
	bool _hooked = false;

	Vector2 _hookPos;

};
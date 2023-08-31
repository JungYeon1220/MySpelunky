#pragma once
class Rope : public Item
{
public:
	Rope();
	virtual ~Rope();

	void Update();
	void Render();

private:
	int length;

};
#pragma once
class Item
{
public:
	Item();
	~Item();

	void Update();
	void Render();

private:
	shared_ptr<Transform> _transform;

};

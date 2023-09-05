#pragma once
class Rope : public Item
{
public:
	Rope();
	virtual ~Rope();

	void Update();
	void Render();

	void DropRope();
private:
	void CreateAction();
	int _length = 4;
	int _curLength = 0;
	bool _hooked = false;
	bool _dropEnd = false;

	vector<shared_ptr<RectCollider>> _ropeCols;

	Vector2 _hookPos;
	shared_ptr<Action> _action;


};
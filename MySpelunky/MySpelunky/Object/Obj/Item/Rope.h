#pragma once
class Rope : public Item
{
public:
	Rope();
	virtual ~Rope();

	void Update();
	void Render();

	void Reset();

	void DropRope();

	vector<shared_ptr<RectCollider>> GetColliders();
	int GetCurLength() { return _curLength; }
	int& GetLength() { return _length; }
	bool& DropEnd() { return _dropEnd; }

	bool& IsHooked() { return _hooked; }
private:
	void CreateAction();
	int _length = 5;
	int _curLength = 1;
	bool _hooked = false;
	bool _dropEnd = false;

	vector<shared_ptr<RectCollider>> _ropeCols;

	Vector2 _hookPos;
	shared_ptr<Action> _action;

};
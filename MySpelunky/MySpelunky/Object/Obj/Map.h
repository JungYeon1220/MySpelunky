#pragma once
class Map
{
public:
	Map();
	~Map();

	void Update();
	void BehindRender();
	void FrontRender();
	void PostRender();

	int GetRopeLength(Vector2 pos);

	map<string, vector<shared_ptr<Tile>>>& GetTypeTiles() { return _types; }
	vector<vector<int>> GetLayout() { return _layout; }
	vector<shared_ptr<Tile>>& GetMovables() { return _types["Movable"]; }
	Vector2 GetStartPos() { return _startPos->GetWorldPos(); }
	Vector2 GetEndPos() { return _endPos->GetWorldPos(); }

	Vector2 PoolCount() { return Vector2(_poolCountX, _poolCountY); }
	void CreateStage();

private:
	vector<vector<int>> GetStartRoom() { return _startRooms[rand() % (_startRooms.size())]; }
	vector<vector<int>> GetEndRoom() { return _endRooms[rand() % (_endRooms.size())]; }
	vector<vector<int>> GetRightRoom() { return _rightRooms[rand() % (_rightRooms.size())]; }
	vector<vector<int>> GetWrongRoom() { return _wrongRooms[rand() % (_wrongRooms.size())]; }
	vector<vector<int>> GetUpRoom() { return _upRooms[rand() % (_upRooms.size())]; }
	vector<vector<int>> GetDownRoom() { return _downRooms[rand() % (_downRooms.size())]; }

	void CreateRooms();
	void CreateRoomLayout();
	void CreateLevelLayout();

	shared_ptr<Transform> _bgTrans;
	shared_ptr<Quad> _bg;

	shared_ptr<Quad> _startDoor;
	shared_ptr<Quad> _endDoor;

	int _poolCountX = 44;
	int _poolCountY = 36;

	vector<vector<int>> _roomLayout;
	vector<vector<int>> _layout;
	//vector<vector<shared_ptr<class Tile>>> _tileMap;

	map<string, vector<shared_ptr<class Tile>>> _types;

	shared_ptr<Transform> _startPos;
	shared_ptr<Transform> _endPos;

	vector<vector<vector<int>>> _startRooms;
	vector<vector<vector<int>>> _endRooms;
	vector<vector<vector<int>>> _rightRooms;
	vector<vector<vector<int>>> _wrongRooms;
	vector<vector<vector<int>>> _upRooms;
	vector<vector<vector<int>>> _downRooms;
};


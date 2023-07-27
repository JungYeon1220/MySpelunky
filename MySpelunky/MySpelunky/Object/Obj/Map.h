#pragma once
class Map
{
public:
	Map();
	~Map();

	void Update();
	void Render();

	vector<vector<int>> GetStartRoom() { return _startRooms[rand() % (_startRooms.size())]; }
	vector<vector<int>> GetEndRoom() { return _endRooms[rand() % (_endRooms.size())]; }
	vector<vector<int>> GetRightRoom() { return _rightRooms[rand() % (_rightRooms.size())]; }
	vector<vector<int>> GetWrongRoom() { return _wrongRooms[rand() % (_wrongRooms.size())]; }
	vector<vector<int>> GetUpRoom() { return _upRooms[rand() % (_upRooms.size())]; }
	vector<vector<int>> GetDownRoom() { return _downRooms[rand() % (_downRooms.size())]; }

	shared_ptr<class Tile> GetTile(int x, int y) { return _tileMap[y][x]; }
	vector<vector<shared_ptr<class Tile>>>& GetTiles() { return _tileMap; }

	Vector2 PoolCount() { return Vector2(_poolCountX, _poolCountY); }
private:
	void CreateRooms();
	void CreateRoomLayout();

	shared_ptr<Transform> _bgTrans;
	shared_ptr<Quad> _bg;

	int _poolCountX = 44;
	int _poolCountY = 36;

	vector<vector<int>> _roomLayout;
	vector<vector<int>> _layout;
	vector<vector<shared_ptr<class Tile>>> _tileMap;

	Vector2 _startPos;
	Vector2 _endPos;

	vector<vector<vector<int>>> _startRooms;
	vector<vector<vector<int>>> _endRooms;
	vector<vector<vector<int>>> _rightRooms;
	vector<vector<vector<int>>> _wrongRooms;
	vector<vector<vector<int>>> _upRooms;
	vector<vector<vector<int>>> _downRooms;
	
};


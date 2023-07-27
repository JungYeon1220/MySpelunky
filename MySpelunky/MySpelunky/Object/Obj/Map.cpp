#include "framework.h"
#include "Map.h"
#include "Tile/Normal.h"
#include "Tile/Unbreakable.h"
#include "Tile/Ladder.h"
#include "Tile/OneWay.h"
#include "Tile/Spike.h"
#include "Tile/Skeleton.h"
#include "Tiles.h"

Map::Map()
{
	_bgTrans = make_shared<Transform>();
	_bg = make_shared<Quad>(Vector2(400.0f,400.0f),L"Resource/Texture/bg_cave.png");

    vector<vector<int>> temp;
    temp.resize(4);
    for (int i = 0; i < 4; i++)
        temp[i].resize(4);

    vector<int> order;
    for (int i = 0; i < 5; i++)
        order.push_back(rand() % 4);

    int start = rand() % 4;

    Vector2 here = Vector2(start, 0);
    while (true)
    {
        temp[here.y][here.x] = 1;
        int random = rand() % 5;
        if (random == 0 || random == 1)
        {
        }
    }

	_layout.resize(_poolCountY);
	for (int i = 0; i < _poolCountY; i++)
	{
		_layout[i].resize(_poolCountX);
		for (int j = 0; j < _poolCountX; j++)
		{
			if (i < 2 || i > _poolCountY - 3 || j < 2 || j > _poolCountX - 3)
				_layout[i][j] = 99;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			int y = i * 8 + 2;
			int x = j * 10 + 2;
			for (int m = y; m < y + 8; m++)
			{
				for (int n = x; n < x + 10; n++)
				{
					int a = road[(m - 2) % 8][(n - 2) % 10];
					_layout[m][n] = a;
				}
			}
		}
	}

	_tileMap.resize(_poolCountY);
	for (int i = 0; i < _poolCountY; i++)
	{
		_tileMap.reserve(_poolCountX);
		for (int j = 0; j < _poolCountX; j++)
		{
			if (_layout[i][j] == 0)
			{
				_tileMap[i].push_back(nullptr);
				continue;
			}

			shared_ptr<Tile> tile;

			if (_layout[i][j] == 1)
			{
				tile = make_shared<Normal>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
				if (_layout[i - 1][j] != 1 && _layout[i - 1][j] != 99)
				{
					if (_layout[i - 1][j] == 4)
						dynamic_pointer_cast<Normal>(tile)->SetSpikePebble();
					else
						dynamic_pointer_cast<Normal>(tile)->PebbleUp();
					dynamic_pointer_cast<Normal>(tile)->CanGrab() = true;
				}
				if (_layout[i + 1][j] != 1 && _layout[i + 1][j] != 99)
					dynamic_pointer_cast<Normal>(tile)->PebbleDown();
				if (_layout[i][j + 1] != 1 && _layout[i][j + 1] != 99)
				{
					dynamic_pointer_cast<Normal>(tile)->PebbleRight();
					if(dynamic_pointer_cast<Normal>(tile)->CanGrab() == true)
						dynamic_pointer_cast<Normal>(tile)->PebbleGrabRight();
				}
				if (_layout[i][j - 1] != 1 && _layout[i][j - 1] != 99)
				{
					dynamic_pointer_cast<Normal>(tile)->PebbleLeft();
					if (dynamic_pointer_cast<Normal>(tile)->CanGrab() == true)
						dynamic_pointer_cast<Normal>(tile)->PebbleGrabLeft();
				}

			}
			else if(_layout[i][j] == 3)
				tile = make_shared<OneWay>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
			else if (_layout[i][j] == 99)
			{
				tile = make_shared<Unbreakable>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
				dynamic_pointer_cast<Unbreakable>(tile)->SetRandomTile(j, i);

				if (i == _poolCountY - 2 && j >= 2 && j <= _poolCountX - 3)
				{
					dynamic_pointer_cast<Unbreakable>(tile)->PebbleUp();
				}
				if (i == 1 && j >= 2 && j <= _poolCountX - 3)
				{
					dynamic_pointer_cast<Unbreakable>(tile)->PebbleDown();
				}
				if (j == 1 && i >= 2 && i <= _poolCountY - 3)
				{
					dynamic_pointer_cast<Unbreakable>(tile)->PebbleRight();
				}
				if (j == _poolCountX - 2 && i >= 2 && i <= _poolCountY - 3)
				{
					dynamic_pointer_cast<Unbreakable>(tile)->PebbleLeft();
				}
			}
			else if (_layout[i][j] == 4)
			{
				tile = make_shared<Spike>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
				if (_layout[i + 1][j] == 5)
				{
					dynamic_pointer_cast<Spike>(tile)->SetSkeletonSpike();
				}
			}
			else if (_layout[i][j] == 5)
			{
				tile = make_shared<Skeleton>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
				if (_layout[i - 1][j] != 5)
				{
					if (_layout[i - 1][j] == 4)
					{
						dynamic_pointer_cast<Skeleton>(tile)->SetSpikePebble();
					}
					else
					{
						dynamic_pointer_cast<Skeleton>(tile)->PebbleUp();
					}
				}
			}

			_tileMap[i].push_back(tile);
		}
	}
}

Map::~Map()
{
}

void Map::Update()
{
	for (auto tileArr : _tileMap)
	{
		for (auto tile : tileArr)
		{
			if (tile == nullptr)
				continue;
			tile->Update();
		}
	}
}

void Map::Render()
{
	for (auto tileArr : _tileMap)
	{
		for (auto tile : tileArr)
		{
			if (tile == nullptr)
				continue;
			if (tile->GetType() == Tile::Type::UNBREAKABLE)
				continue;
			tile->Render();
		}
	}
	for (auto tileArr : _tileMap)
	{
		for (auto tile : tileArr)
		{
			if (tile == nullptr)
				continue;
			if (tile->GetType() == Tile::Type::UNBREAKABLE)
				tile->Render();
		}
	}
}

void Map::CreateRooms()
{
    vector<vector<int>> start0 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 6, 0, 0, 0, 0},
        {0, 5, 5, 1, 1, 1, 1, 5, 5, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    //�Ʒ� �̵��� ���� �Ʒ��� �� �߾�2ĭ�� 0
    vector<vector<int>> start1 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 6, 0, 0, 2, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 4, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    vector<vector<int>> start2 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 2, 0, 0, 6, 0, 0, 1, 0},
        {0, 0, 4, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    //�Ʒ� �Ұ���
    vector<vector<int>> start4 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 6, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    //������ �����´ٸ� ���� ���� 3�� ��� 0
    vector<vector<int>> end0 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 7, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    vector<vector<int>> end1 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 7, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    vector<vector<int>> end2 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 7, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    vector<vector<int>> road0 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 9, 5, 5, 5, 5, 5, 5, 9, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
        {1,99,10,10,10,10,10,10, 1, 1},
        {1, 1, 1, 1,11,11, 1, 1, 1, 1}
    };
    vector<vector<int>> road1 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 1, 1, 0},
        {1, 0, 0, 3, 0, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    vector<vector<int>> road2 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 1, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 5, 1, 1, 1, 1, 5, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    vector<vector<int>> road3 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 5, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    vector<vector<int>> road4 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 8, 0, 0, 0, 0, 5, 0},
        {1, 1, 0, 1, 1, 1, 1, 1, 4, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 5, 0},
        {0, 1, 1, 1, 1, 0, 0, 0, 5, 0},
        {0, 0, 0, 1, 0, 0, 1, 0, 5, 0},
        {1, 0, 0, 0, 0, 1, 1, 0, 5, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    vector<vector<int>> road5 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 1, 1, 0},
        {0, 1, 1, 1, 0, 0, 0, 1, 1, 0},
        {5, 1, 1, 0, 5, 1, 1, 1, 1, 5},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    vector<vector<int>> wrongWay0 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 1, 1, 1, 5},
        {0, 1, 1, 1, 0, 0, 1, 1, 1, 5},
        {0, 1, 1, 1, 0, 0, 1, 1, 1, 5},
        {0, 1, 1, 1, 0, 0, 1, 1, 1, 5},
        {0, 0, 0, 0, 0, 0,99, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    vector<vector<int>> wrongWay1 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 1, 1,99, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 0, 0, 1, 1, 0},
        {1, 1, 8, 1, 1, 0, 0, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 5},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    vector<vector<int>> wrongWay2 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 3, 0, 1, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 5},
        {0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 0, 1, 1, 1, 0},
        {0, 0, 3, 0, 0, 0,99, 1, 1,10},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    vector<vector<int>> wrongWay3 =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 5, 5, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 5, 5, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 5, 5},
        {1, 1,10,10,10,10,10,10,10,10},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    vector<vector<int>> upWay0 =
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,10, 1, 1, 0, 0},
        {0, 0, 0, 0, 0,10, 1, 0, 0, 0},
        {0, 0, 0, 0, 1,11, 1, 0, 0, 5},
        {0, 0, 0, 0, 1,11, 1, 0, 0, 5},
        {5, 5, 5, 1, 1,11, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 0, 0, 1}
    };

    vector<vector<int>> downWay0 =
    {
        {1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 1, 5},
        {0, 0, 0, 0, 0, 0, 0,11, 1, 5},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    _startRooms.push_back(start0);
    _startRooms.push_back(start1);
    _startRooms.push_back(start2);
    _startRooms.push_back(start4);

    _endRooms.push_back(end0);
    _endRooms.push_back(end1);
    _endRooms.push_back(end2);

    _rightRooms.push_back(road0);
    _rightRooms.push_back(road1);
    _rightRooms.push_back(road2);
    _rightRooms.push_back(road3);
    _rightRooms.push_back(road4);
    _rightRooms.push_back(road5);

    _wrongRooms.push_back(wrongWay0);
    _wrongRooms.push_back(wrongWay1);
    _wrongRooms.push_back(wrongWay2);
    _wrongRooms.push_back(wrongWay3);

    _upRooms.push_back(upWay0);

    _downRooms.push_back(downWay0);
}

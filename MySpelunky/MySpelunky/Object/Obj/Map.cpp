#include "framework.h"
#include "Map.h"
#include "Tile/Normal.h"
#include "Tile/Unbreakable.h"
#include "Tile/Ladder.h"
#include "Tile/OneWay.h"
#include "Tile/Spike.h"
#include "Tile/Skeleton.h"
#include "Tile/Wooden.h"
#include "Tile/Movable.h"
#include "Tiles.h"

Map::Map()
{
	_bgTrans = make_shared<Transform>();
	_bg = make_shared<Quad>(Vector2(400.0f * 10.0f,400.0f * 8.0f),L"Resource/Texture/bg_cave.png",Vector2(10.0f, 8.0f));

    CreateRooms();
    CreateRoomLayout();
	CreateLevelLayout();

	_tileMap.resize(_poolCountY);
	for (int i = 0; i < _poolCountY; i++)
	{
		_tileMap.reserve(_poolCountX);
		for (int j = 0; j < _poolCountX; j++)
		{
			int type = _layout[i][j];

			shared_ptr<Tile> tile;



			if (type == 0 || type == 3 || type == 6 || type == 7 || type == 99)
			{
				_tileMap[i].push_back(nullptr);
				continue;
			}

			if (type == 1)
			{
				tile = make_shared<Normal>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
				if (_layout[i - 1][j] != 1 && _layout[i - 1][j] != 50)
				{
					if (_layout[i - 1][j] == 10)
						dynamic_pointer_cast<Normal>(tile)->SetSpikePebble();
					else
						dynamic_pointer_cast<Normal>(tile)->PebbleUp();
					tile->CanGrab() = true;
				}
				if (_layout[i + 1][j] != 1 && _layout[i + 1][j] != 50)
					dynamic_pointer_cast<Normal>(tile)->PebbleDown();
				if (_layout[i][j + 1] != 1 && _layout[i][j + 1] != 50)
				{
					dynamic_pointer_cast<Normal>(tile)->PebbleRight();
					if (dynamic_pointer_cast<Normal>(tile)->CanGrab() == true)
					{
						dynamic_pointer_cast<Normal>(tile)->PebbleGrabRight();
						tile->LedgeRight() = true;
					}
				}
				if (_layout[i][j - 1] != 1 && _layout[i][j - 1] != 50)
				{
					dynamic_pointer_cast<Normal>(tile)->PebbleLeft();
					if (dynamic_pointer_cast<Normal>(tile)->CanGrab() == true)
					{
						dynamic_pointer_cast<Normal>(tile)->PebbleGrabLeft();
						tile->LedgeLeft() = true;
					}
				}
				_types["Normal"].push_back(tile);
			}
			else if (type == 5)
			{
				tile = make_shared<OneWay>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));

				int count = 1;
				while (true)
				{
					if (_layout[i + count][j] == 1 || _layout[i + count][j] == 11 || _layout[i + count][j] == 5)
						break;
					count++;
				}
				dynamic_pointer_cast<OneWay>(tile)->LegCount() = count - 1;

				_types["Oneway"].push_back(tile);
			}
			else if (type == 50)
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

				_types["Unbreakable"].push_back(tile);
			}
			else if (type == 10)
			{
				tile = make_shared<Spike>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
				if (_layout[i + 1][j] == 11)
				{
					dynamic_pointer_cast<Spike>(tile)->SetSkeletonSpike();
				}

				_types["Spike"].push_back(tile);
			}
			else if (type == 11)
			{
				tile = make_shared<Skeleton>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
				if (_layout[i - 1][j] == 0)
					tile->CanGrab() = true;
				if (_layout[i - 1][j] != 11)
				{
					if (_layout[i - 1][j] == 10)
					{
						dynamic_pointer_cast<Skeleton>(tile)->SetSpikePebble();
					}
					else
					{
						dynamic_pointer_cast<Skeleton>(tile)->PebbleUp();
					}
				}

				_types["Skeleton"].push_back(tile);
			}
            else if (type == 2)
            {
                tile = make_shared<Ladder>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
                if (_layout[i - 1][j] != 2 && _layout[i - 1][j] != 4)
                {
                    dynamic_pointer_cast<Ladder>(tile)->SetTop();
                }
                else if (_layout[i + 1][j] != 2 && _layout[i + 1][j] != 4)
                {
                    dynamic_pointer_cast<Ladder>(tile)->SetBottom();
                }

				_types["Ladder"].push_back(tile);
            }
            else if (type == 4)
            {
                tile = make_shared<Ladder>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
                dynamic_pointer_cast<Ladder>(tile)->SetOneWay();

				_types["Ladder"].push_back(tile);
            }
            else if (type == 9)
            {
                tile = make_shared<Wooden>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
				if (_layout[i - 1][j] == 0)
					tile->CanGrab() = true;

				_types["Wooden"].push_back(tile);
            }
            else if (type == 8)
            {
                tile = make_shared<Movable>(Vector2(j * 100.0f, (_poolCountY - 1 - i) * 100.0f));
				tile->CanGrab() = true;
				_types["Movable"].push_back(tile);
            }

			_tileMap[i].push_back(tile);
			tile->Update();
		}
	}

    _bgTrans->SetPosition(Vector2(_tileMap[0][_poolCountX - 1]->GetCollider()->GetWorldPos().x / 2, _tileMap[0][_poolCountX - 1]->GetCollider()->GetWorldPos().y / 2));
	_bgTrans->Update();
	_bg->Update();
}

Map::~Map()
{
}

void Map::Update()
{
    for (int i = 0; i < _types["Movable"].size(); i++)
    {
		_types["Movable"][i]->Update();
        bool check = false;
        for (auto tileArr : _tileMap)
        {
            for (auto tile : tileArr)
            {
                if (tile == nullptr)
                    continue;

				float x = _types["Movable"][i]->GetCollider()->GetWorldPos().x - tile->GetCollider()->GetWorldPos().x;
				float y = _types["Movable"][i]->GetCollider()->GetWorldPos().y - tile->GetCollider()->GetWorldPos().y;
				if (x * x + y * y > 40000.0f)
					continue;

                if (tile->GetType() == Tile::Type::MOVABLE)
                    continue;

				if (tile->GetCollider()->IsCollision(dynamic_pointer_cast<Movable>(_types["Movable"][i])->GetMovableCollider()))
				{
					if (tile->GetCollider()->Block(_types["Movable"][i]->GetCollider()))
					{
						if (tile->GetCollider()->GetWorldPos().y < _types["Movable"][i]->GetCollider()->GetWorldPos().y)
							check = true;
					}
				}
            }
        }

		for (int j = 0; j < _types["Movable"].size(); j++)
		{
			if (i == j)
				continue;

			if (_types["Movable"][j]->GetCollider()->IsCollision(dynamic_pointer_cast<Movable>(_types["Movable"][i])->GetMovableCollider()))
			{
				if (_types["Movable"][j]->GetCollider()->Block(_types["Movable"][i]->GetCollider()))
				{
					if (_types["Movable"][j]->GetCollider()->GetWorldPos().y < _types["Movable"][i]->GetCollider()->GetWorldPos().y)
						check = true;
				}
			}
		}

        if (check == false)
        {
            dynamic_pointer_cast<Movable>(_types["Movable"][i])->IsFalling() = true;
        }
        else
        {
            dynamic_pointer_cast<Movable>(_types["Movable"][i])->IsFalling() = false;
        }
    }
}

void Map::BehindRender()
{
	_bgTrans->SetWorldBuffer(0);
	_bg->Render();

	for (auto ladder : _types["Ladder"])
		ladder->Render();
	for (auto oneway : _types["Oneway"])
		oneway->Render();
	for (auto spike : _types["Spike"])
		spike->Render();

}

void Map::FrontRender()
{
	for (auto movable : _types["Movable"])
		movable->Render();
	for (auto skeleton : _types["Skeleton"])
		skeleton->Render();
	for (auto wooden : _types["Wooden"])
		wooden->Render();
	for (auto normal : _types["Normal"])
		normal->Render();
	for (auto unbreakable : _types["Unbreakable"])
		unbreakable->Render();
}

void Map::PostRender()
{
}

void Map::CreateRooms()
{
//0: 비어있음
//1 : 벽
//2 : 사다리
//3 : 거미줄
//4. 발판 사다리
//5. 발판
//6. 시작지점
//7. 끝지점
//8. 확정 이동블럭
//9. 나무 벽
//10. 가시
//11. 해골
//99. 아이템(상자, 저주항아리, 뽀찌, 보석상자 등)

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

    //아래 이동시 가장 아래줄 중 중앙2칸만 0
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

    //아래 불가능
    vector<vector<int>> start3 =
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

    //위에서 내려온다면 가장 위의 3줄 모두 0
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
        {0, 0, 0, 8, 0, 0, 0, 0, 2, 0},
        {1, 1, 0, 1, 1, 1, 1, 1, 4, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
        {0, 1, 1, 1, 1, 0, 0, 0, 2, 0},
        {0, 0, 0, 1, 0, 0, 1, 0, 2, 0},
        {1, 0, 0, 0, 0, 1, 1, 0, 2, 0},
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
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
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
    _startRooms.push_back(start3);

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

void Map::CreateRoomLayout()
{
    _roomLayout.resize(4);
    for (int i = 0; i < 4; i++)
        _roomLayout[i].resize(4);

    int start = rand() % 4;
    int dir = rand() % 2;

    Vector2 here = Vector2(start, 0);
    while (true)
    {
        if (_roomLayout[here.y][here.x] == 0)
            _roomLayout[here.y][here.x] = 1;

        int random = rand() % 5;
        if (here.x == 0)
        {
            if (dir == 0)
                random = 4;
            dir = 1;
        }
        else if (here.x == 3)
        {
            if (dir == 1)
                random = 4;
            dir = 0;
        }
        else if (_roomLayout[here.y][here.x] == 3)
        {
            if (random == 0 || random == 1)
                dir = 0;
            else if (random == 2 || random == 3)
                dir = 1;
        }

        Vector2 there;
        if (random < 4)
        {
            if (dir == 0)
                there = here - RIGHT_VECTOR;
            else if (dir == 1)
                there = here + RIGHT_VECTOR;
        }
        else if (random == 4)
        {
            if (here.y == 3)
            {
                _roomLayout[here.y][here.x] = 5;
                break;
            }

            there = here + UP_VECTOR;
            _roomLayout[here.y][here.x] = 2;
            _roomLayout[there.y][there.x] = 3;
        }

        here = there;
    }

    _roomLayout[0][start] = 4;
}

void Map::CreateLevelLayout()
{
	_layout.resize(_poolCountY);
	for (int i = 0; i < _poolCountY; i++)
	{
		_layout[i].resize(_poolCountX);
		for (int j = 0; j < _poolCountX; j++)
		{
			if (i < 2 || i > _poolCountY - 3 || j < 2 || j > _poolCountX - 3)
				_layout[i][j] = 50;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int y = i * 8 + 2;
			int x = j * 10 + 2;

			vector<vector<int>> temp;
			int type = _roomLayout[i][j];
			if (type == 0)
				temp = GetWrongRoom();
			else if (type == 1)
				temp = GetRightRoom();
			else if (type == 2)
			{
				temp = GetUpRoom();
			}
			else if (type == 3)
			{
				temp = GetDownRoom();
				if (_roomLayout[i - 1][j] == 4)
				{
					temp = GetRightRoom();
					temp[0][4] = 0;
					temp[0][5] = 0;
				}
			}
			else if (type == 4)
			{
				temp = GetStartRoom();
				if (j == 0)
				{
					if (_roomLayout[i][j + 1] == 0)
					{
						temp = _startRooms[1];
						temp[7][4] = 0;
						temp[7][5] = 0;
					}
				}
				else if (j == 3)
				{
					if (_roomLayout[i][j - 1] == 0)
					{
						temp = _startRooms[1];
						temp[7][4] = 0;
						temp[7][5] = 0;
					}
				}
				else if (_roomLayout[i][j + 1] == 0 && _roomLayout[i][j - 1] == 0)
				{
					temp = _startRooms[1];
					temp[7][4] = 0;
					temp[7][5] = 0;
				}
			}
			else if (type == 5)
			{
				temp = GetEndRoom();
				if (_roomLayout[i - 1][j] == 2)
				{
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 10; j++)
							temp[i][j] = 0;
					}
				}
			}

			for (int m = y; m < y + 8; m++)
			{
				for (int n = x; n < x + 10; n++)
				{
					int a = temp[(m - 2) % 8][(n - 2) % 10];
					if (a == 6)
						_startPos = Vector2(n, m);
					if (a == 7)
						_endPos = Vector2(n, m);
					_layout[m][n] = a;
				}
			}
		}
	}
}

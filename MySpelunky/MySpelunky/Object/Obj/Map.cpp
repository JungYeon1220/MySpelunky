#include "framework.h"
#include "Map.h"
#include "Tile/Normal.h"
#include "Tile/Unbreakable.h"
#include "Tile/Ladder.h"
#include "Tile/OneWay.h"
#include "Tiles.h"

Map::Map()
{
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

void Map::CreateTiles()
{

}

#include "framework.h"
#include "Snake.h"

Snake::Snake()
{
	_size = Vector2(50.0f, 50.0f);
	_col = make_shared<RectCollider>(_size);
	_rangeCol = make_shared<RectCollider>(Vector2(100.0f, 50.0f));
	_rangeCol->GetTransform()->SetParent(_col->GetTransform());
	_rangeCol->GetTransform()->SetPosition(Vector2(50.0f, 0.0f));
	_transform = make_shared<Transform>();
	_transform->SetParent(_col->GetTransform());
	_transform->SetPosition(Vector2(0.0f, 7.0f));

	CreateAction();

	SetAction(State::IDLE);

	_maxHp = 1;
	_hp = 1;
	_moveSpeed = 80.0f;
	_maxDuration = (float)(rand() % 2) + rand() / static_cast<float>(RAND_MAX);
	if (_maxDuration < 0.5f)
		_maxDuration = 0.5f;

	Left();
}

Snake::Snake(Vector2 pos)
{
	_size = Vector2(50.0f, 50.0f);
	_col = make_shared<RectCollider>(_size);
	_col->GetTransform()->SetPosition(pos);
	_rangeCol = make_shared<RectCollider>(Vector2(100.0f, 50.0f));
	_rangeCol->GetTransform()->SetParent(_col->GetTransform());
	_rangeCol->GetTransform()->SetPosition(Vector2(50.0f, 0.0f));
	_transform = make_shared<Transform>();
	_transform->SetParent(_col->GetTransform());
	_transform->SetPosition(Vector2(0.0f, 7.0f));

	CreateAction();

	SetAction(State::IDLE);

	_maxHp = 1;
	_hp = 1;
	_moveSpeed = 80.0f;
	_maxDuration = (float)(rand() % 2) + rand() / static_cast<float>(RAND_MAX);
	if (_maxDuration < 0.5f)
		_maxDuration = 0.5f;

	Left();
}

Snake::~Snake()
{
}

void Snake::Update()
{
	if (_isActive == false)
		return;
	if (_isDead == true)
		return;

	Move();
	
	_actions[_curState]->Update();
	Monster::Update();
}

void Snake::Render()
{
	if (_isActive == false)
		return;
	if (_isDead == true)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

	_transform->SetWorldBuffer(0);
	if (_isLeft)
		SPRITEMANAGER->GetSprite("Snake")->SetLeft();
	else
		SPRITEMANAGER->GetSprite("Snake")->SetRight();
	SPRITEMANAGER->Render("Snake", _actions[_curState]->GetCurClip());

	_col->Render();

}

void Snake::TakeDamage(int value)
{
	Monster::TakeDamage(value);

	SOUND->Play("SnakeDie");
}

bool Snake::TileInteract(shared_ptr<Tile> tile)
{
	bool Landcheck = false;

	if (tile->Block(_col))
	{
		Vector2 tilePos = tile->GetCollider()->GetWorldPos();
		if ((_col->GetWorldPos().y + _size.y * 0.5f > tilePos.y - 50.0f
			&& _col->GetWorldPos().y - _size.y * 0.5f < tilePos.y + 50.0f) == false)
			Landcheck = true;
		else
		{
			if (_isMoving == true)
				Reverse();
		}

		if (_col->GetWorldPos().x < tilePos.x + 50.0f && _col->GetWorldPos().x > tilePos.x - 50.0f)
		{
			if (tile->LedgeRight() == true)
			{
				if (tilePos.x + 50.0f < _col->GetWorldPos().x + _size.x * 0.5f)
				{
					if (_isMoving == true)
					{
						Reverse();
						_col->GetTransform()->AddVector2(Vector2(-0.1f, 0.0f));
					}
				}
			}

			if (tile->LedgeLeft() == true)
			{
				if (tilePos.x - 50.0f > _col->GetWorldPos().x - _size.x * 0.5f)
				{
					if (_isMoving == true)
					{
						Reverse();
						_col->GetTransform()->AddVector2(Vector2(0.1f, 0.0f));
					}
				}
			}
		}
	}

	return Landcheck;
}

void Snake::Land(bool check)
{
	if (check == false)
	{
		_isFalling = true;
	}
	else
	{
		_isFalling = false;
		_curSpeed = 0.0f;
	}
}

void Snake::Left()
{
	_dir = -RIGHT_VECTOR;
	_rangeCol->GetTransform()->SetPosition(Vector2(-50.0f, 0.0f));
	_isLeft = true;
}

void Snake::Right()
{
	_dir = RIGHT_VECTOR;
	_rangeCol->GetTransform()->SetPosition(Vector2(50.0f, 0.0f));
	_isLeft = false;
}

void Snake::Reverse()
{
	if (_isLeft == true)
		Right();
	else
		Left();
}

void Snake::Move()
{
	if (_isAttack == true)
	{
		if (_actions[State::ATTACK]->GetCurIndex() == 2)
		{
			_curSpeed = 500.0f;
			SOUND->Play("SnakeAttack");
		}

		return;
	}

	_duration += DELTA_TIME;
	if (_duration >= _maxDuration)
	{
		if (_isMoving == false)
		{
			_isMoving = true;
			int random = rand() % 2;
			if (random == 0)
				Reverse();
		}
		else
			_isMoving = false;

		_duration = 0.0f;
		_maxDuration = MathUtility::RandomFloat(0.5f, 2.0f);
	}

	if (_isMoving == true)
	{
		SetAction(State::MOVE);
		_curSpeed = _moveSpeed;
	}
	else
	{
		SetAction(State::IDLE);
		_curSpeed = 0.0f;
	}
}

bool Snake::SetTarget(shared_ptr<class Player> player)
{
	if (_rangeCol->IsCollision(player->GetHitCollider()))
	{
		if (_inRange == false)
		{
			_isAttack = true;
			SetAction(State::ATTACK);
			_inRange = true;
			_isMoving = false;
			_duration = 0.0f;
		}
	}
	else
		_inRange = false;

	return _inRange;
}

void Snake::EndAttack()
{
	_isAttack = false;
	SetAction(State::IDLE);
}

void Snake::SetAction(State state)
{
	if (_curState == state)
		return;

	_oldState = _curState;
	_actions[_oldState]->Reset();
	_actions[_oldState]->Pause();

	_curState = state;
	_actions[_curState]->Play();
}

void Snake::CreateAction()
{
	shared_ptr<SRV> srv = ADD_SRV(L"Resource/Texture/snake.png");
	Vector2 imageSize = srv->GetImageSize();
	Vector2 maxFrame = Vector2(4, 3);
	Vector2 size;
	size.x = imageSize.x / maxFrame.x;
	size.y = imageSize.y / maxFrame.y;

	{
		vector<Action::Clip> clips;

		Vector2 startPos = Vector2(0, 0);
		Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
		clips.push_back(clip);

		shared_ptr<Action> action = make_shared<Action>(clips, "IDLE", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;

		for (int i = 0; i < 4; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 0.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "MOVE", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;

		for (int j = 1; j < 3; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * j / maxFrame.y);
				Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
				clips.push_back(clip);
			}
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "ATTACK	", Action::END);
		action->SetEndEvent(std::bind(&Snake::EndAttack, this));
		_actions.push_back(action);
	}
}

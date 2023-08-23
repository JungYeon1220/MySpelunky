#include "framework.h"
#include "Mosquito.h"
#include "../Player.h"

Mosquito::Mosquito()
{
	_size = Vector2(50.0f, 50.0f);
	_col = make_shared<RectCollider>(_size);
	_rangeCol = make_shared<RectCollider>(Vector2(240.0f, 50.0f));
	_rangeCol->GetTransform()->SetParent(_col->GetTransform());
	_rangeCol->GetTransform()->SetPosition(Vector2(120.0f, 0.0f));
	_transform = make_shared<Transform>();
	_transform->SetParent(_col->GetTransform());
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/mosquito.png", Vector2(3, 3), Vector2(70.0f, 70.0f));

	CreateAction();

	SetAction(State::IDLE);
	_actions[_curState]->Play();

	_hp = 1;
	_moveSpeed = 80.0f;
	_maxDuration = (float)(rand() % 2 + 1) + rand() / static_cast<float>(RAND_MAX);

	float angle = MathUtility::RandomFloat(0.0f, 2.0f) * PI;
	_dir = Vector2(cosf(angle) - sinf(angle), sinf(angle) + cosf(angle));
	_dir.Normalize();
}

Mosquito::Mosquito(Vector2 pos)
{
	_size = Vector2(50.0f, 50.0f);
	_col = make_shared<RectCollider>(_size);
	_col->GetTransform()->SetPosition(pos);
	_rangeCol = make_shared<RectCollider>(Vector2(240.0f, 50.0f));
	_rangeCol->GetTransform()->SetParent(_col->GetTransform());
	_rangeCol->GetTransform()->SetPosition(Vector2(120.0f, 0.0f));
	_transform = make_shared<Transform>();
	_transform->SetParent(_col->GetTransform());
	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/mosquito.png", Vector2(3, 3), Vector2(70.0f, 70.0f));

	CreateAction();

	SetAction(State::IDLE);
	_actions[_curState]->Play();

	_hp = 1;
	_moveSpeed = 80.0f;
	_maxDuration = (float)(rand() % 2 + 1) + rand() / static_cast<float>(RAND_MAX);

	float angle = MathUtility::RandomFloat(0.0f, 2.0f) * PI;
	_dir = Vector2(cosf(angle) - sinf(angle), sinf(angle) + cosf(angle));
	_dir.Normalize();
	
}

Mosquito::~Mosquito()
{
}

void Mosquito::Update()
{
	if (_isDead == true)
		return;

	Move();

	_col->GetTransform()->AddVector2(_dir * _curSpeed * DELTA_TIME);

	_col->Update();
	_rangeCol->Update();
	_transform->Update();

	_actions[_curState]->Update();
	_sprite->SetCurClip(_actions[_curState]->GetCurClip());
	_sprite->Update();
}

void Mosquito::Render()
{
	Monster::Render();
}

bool Mosquito::TileInteract(shared_ptr<Tile> tile)
{
	return tile->Block(_col);
}

void Mosquito::Move()
{
	if (_isAttack == true)
	{
		_curSpeed = 300.0f;

		_attackTime += DELTA_TIME;
		if (_attackTime >= 2.0f)
		{
			_isAttack = false;
			_curSpeed = 0.0f;
			SetAction(State::IDLE);
			_attackTime = 0.0f;
		}

		return;
	}

	_duration += DELTA_TIME;
	if (_duration >= _maxDuration)
	{
		if (_isMoving == false)
		{
			_isMoving = true;
			float angle = MathUtility::RandomFloat(0.0f, 2.0f) * PI;
			_dir = Vector2(cosf(angle) - sinf(angle), sinf(angle) + cosf(angle));
			_dir.Normalize();
		}
		else
			_isMoving = false;

		_duration = 0.0f;
		_maxDuration = MathUtility::RandomFloat(0.5f, 2.0f);
	}

	if (_isMoving == true)
	{
		_curSpeed = _moveSpeed;
	}
	else
	{
		_curSpeed = 0.0f;
	}

	if (_dir.x < 0.0f)
	{
		_sprite->SetLeft();
		_rangeCol->GetTransform()->SetPosition(Vector2(-120.0f, 0.0f));
		_isLeft = true;
	}
	else
	{
		_sprite->SetRight();
		_rangeCol->GetTransform()->SetPosition(Vector2(120.0f, 0.0f));
		_isLeft = false;
	}

}

void Mosquito::SetTarget(shared_ptr<class Player> player)
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

			if (_isLeft == true)
			{
				_dir = -RIGHT_VECTOR;
			}
			else
			{
				_dir = RIGHT_VECTOR;
			}
		}
	}
	else
		_inRange = false;
}

void Mosquito::SetAction(State state)
{
	if (_curState == state)
		return;

	_oldState = _curState;
	_actions[_oldState]->Reset();
	_actions[_oldState]->Pause();

	_curState = state;
	_actions[_curState]->Play();
}

void Mosquito::CreateAction()
{
	shared_ptr<SRV> srv = ADD_SRV(L"Resource/Texture/mosquito.png");
	Vector2 imageSize = srv->GetImageSize();
	Vector2 maxFrame = Vector2(3, 3);
	Vector2 size;
	size.x = imageSize.x / maxFrame.x;
	size.y = imageSize.y / maxFrame.y;

	{
		vector<Action::Clip> clips;

		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				if (i == 0 && j == 0)
					continue;
				if (j == 2 && i == 1)
					break;
				Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * j / maxFrame.y);
				Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
				clips.push_back(clip);
			}
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "IDLE", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;

		Vector2 startPos = Vector2(0.0f, 0.0f);
		Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
		clips.push_back(clip);

		shared_ptr<Action> action = make_shared<Action>(clips, "ATTACK", Action::LOOP);
		_actions.push_back(action);
	}
}

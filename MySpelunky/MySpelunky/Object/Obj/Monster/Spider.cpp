#include "framework.h"
#include "Spider.h"

Spider::Spider()
{
	_size = Vector2(40.0f, 40.0f);
	_col = make_shared<RectCollider>(_size);
	_rangeCol = make_shared<RectCollider>(Vector2(500.0f, 300.0f));
	_rangeCol->GetTransform()->SetParent(_col->GetTransform());
	_transform = make_shared<Transform>();
	_transform->SetParent(_col->GetTransform());
	_transform->SetPosition(Vector2(0.0f, 3.0f));

	CreateAction();

	SetAction(State::IDLE);

	_hp = 1;
	_moveSpeed = 200.0f;
}

Spider::Spider(Vector2 pos)
{
	_size = Vector2(40.0f, 40.0f);
	_col = make_shared<RectCollider>(_size);
	_col->GetTransform()->SetPosition(pos);
	_rangeCol = make_shared<RectCollider>(Vector2(500.0f, 300.0f));
	_rangeCol->GetTransform()->SetParent(_col->GetTransform());
	_transform = make_shared<Transform>();
	_transform->SetParent(_col->GetTransform());
	_transform->SetPosition(Vector2(0.0f, 3.0f));

	CreateAction();

	SetAction(State::IDLE);

	_hp = 1;
	_moveSpeed = 200.0f;
}

Spider::~Spider()
{
}

void Spider::Update()
{
	if (_isDead == true)
		return;

	if (_isFalling == true)
		SetAction(State::JUMP);
	if (_isFalling == false && _isJumping == false)
		SetAction(State::IDLE);

	if (_isFalling == false)
		_curSpeed = 0.0f;

	Jump();

	if (_canJump == false)
	{
		_curJumpTime += DELTA_TIME;
	}
	if (_curJumpTime >= _jumpCoolTime)
	{
		_canJump = true;
		_curJumpTime = 0.0f;
	}

	_actions[_curState]->Update();
	Monster::Update();
}

void Spider::Render()
{
	if (_isDead == true)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

	_transform->SetWorldBuffer(0);
	if (_isLeft)
		SPRITEMANAGER->GetSprite("Spider")->SetLeft();
	else
		SPRITEMANAGER->GetSprite("Spider")->SetRight();
	SPRITEMANAGER->Render("Spider", _actions[_curState]->GetCurClip());
}

bool Spider::TileInteract(shared_ptr<Tile> tile)
{
	bool check = false;

	if (tile->Block(_col))
	{
		Vector2 tilePos = tile->GetCollider()->GetWorldPos();
		if ((_col->GetWorldPos().y + _size.y * 0.5f > tilePos.y - 50.0f
		&& _col->GetWorldPos().y - _size.y * 0.5f < tilePos.y + 50.0f) == false
			&& _col->GetWorldPos().y > tile->GetCollider()->GetWorldPos().y)
			check = true;
		else
		{
			_curSpeed = 0.0f;
		}
	}

	return check;
}

void Spider::Land(bool check)
{
	if (check == false)
	{
		_isFalling = true;
	}
	else
	{
		if (_isFalling == true)
			_isJumping = false;
		_isFalling = false;
		_curSpeed = 0.0f;
	}
}

void Spider::SetAction(State state)
{
	if (_curState == state)
		return;

	_oldState = _curState;
	_actions[_oldState]->Reset();
	_actions[_oldState]->Pause();

	_curState = state;
	_actions[_curState]->Play();
}

void Spider::Jump()
{
	if (_actions[State::JUMP]->GetCurIndex() == 6)
	{
		_curSpeed = _moveSpeed;
		_jumpPower = 1200.0f;
		_isFalling = true;
		_col->GetTransform()->AddVector2(Vector2(0.0f, 0.01f));
		_canJump = false;
	}
	if (_actions[State::JUMP]->GetCurIndex() == 7)
	{
		_actions[State::JUMP]->Pause();
	}
}

bool Spider::SetTarget(shared_ptr<Player> player)
{
	if (_rangeCol->IsCollision(player->GetCollider()))
	{
		_inRange = true;
		if (_canJump == true && _isFalling == false)
		{
			SetAction(State::JUMP);
			_isJumping = true;
			if (player->GetCollider()->GetWorldPos().x < _col->GetWorldPos().x)
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
	{
		_inRange = false;
	}

	return _inRange;
}

void Spider::CreateAction()
{
	shared_ptr<SRV> srv = ADD_SRV(L"Resource/Texture/spider.png");
	Vector2 imageSize = srv->GetImageSize();
	Vector2 maxFrame = Vector2(4, 4);
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
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * j / maxFrame.y);
				Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
				clips.push_back(clip);
			}
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "JUMP", Action::END);
		_actions.push_back(action);
	}
}

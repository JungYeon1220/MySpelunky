#include "framework.h"
#include "Bomb.h"

Bomb::Bomb()
{
	_size = Vector2(35.0f, 35.0f);
	_col = make_shared<RectCollider>(_size);
	_offsetTrans = make_shared<Transform>();
	_offsetTrans->SetParent(_col->GetTransform());
	_offsetTrans->SetPosition(Vector2(0.5f, -2.5f));
	_transform = make_shared<Transform>();
	_transform->SetParent(_offsetTrans);
	_transform->SetPosition(Vector2(-0.5f, 2.5f));
	_range = make_shared<CircleCollider>(150.0f);
	_range->GetTransform()->SetParent(_col->GetTransform());
	_name = "Bomb";
	CreateAction();
}

Bomb::~Bomb()
{
}

void Bomb::Update()
{
	if (_isActive == false)
	{
		_boom = false;
		return;
	}

	if (_isFalling == false && _isActive == true)
	{
		_speed *= 0.9f;
		_rotation = -_speed / 15.0f;
	}

	if (_speed < 0.1f && _speed > -0.1f)
		_speed = 0.0f;

	if (_time >= 1.5f && _time < 2.5f)
		SetAction(State::IMMINENT1);
	if (_time >= 2.5f && _time < 3.0f)
		SetAction(State::INNINENT2);
	if (_time >= 3.0f)
	{
		_isActive = false;
		_isFalling = false;
		_time = 0.0f;
		_speed = 0.0f;
		_jumpPower = 0.0f;
		_boom = true;
		CAMERA->ShakeStart(2.0f, 1.0f);
		EFFECT->Play("Explosion", _col->GetWorldPos());
		SOUND->Play("Explosion");
		SetAction(State::IDLE);
	}

	_time += DELTA_TIME;

	_col->GetTransform()->AddVector2(RIGHT_VECTOR * _speed);
	_offsetTrans->AddAngle(_rotation);
	_actions[_curState]->Update();

	_offsetTrans->Update();
	_range->Update();
	Item::Update();
}

void Bomb::Render()
{
	if (_isActive == false)
		return;
	if (_col->IsCollision(CAMERA->GetViewCollider()) == false)
		return;

	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Item", _actions[_curState]->GetCurClip());
}

void Bomb::Reset()
{
	_isActive = false;
	_boom = false;
	SetAction(IDLE);
	_time = 0.0f;
}

bool Bomb::DestroyTile(shared_ptr<Tile> tile)
{
	if (_boom == false)
		return false;

	if (_range->IsCollision(tile->GetCollider()))
	{
		tile->Destroy();
		return true;
	}

	return false;
}

void Bomb::SetAction(State state)
{
	if (_curState == state)
		return;

	_oldState = _curState;
	_actions[_oldState]->Reset();
	_actions[_oldState]->Pause();

	_curState = state;
	_actions[_curState]->Play();
}

void Bomb::CreateAction()
{
	shared_ptr<SRV> srv = ADD_SRV(L"Resource/Texture/items.png");
	Vector2 imageSize = srv->GetImageSize();
	Vector2 maxFrame = Vector2(16, 16);
	Vector2 size;
	size.x = imageSize.x / maxFrame.x;
	size.y = imageSize.y / maxFrame.y;

	{
		vector<Action::Clip> clips;

		Vector2 startPos = Vector2(0.0f, imageSize.y * 5.0f / maxFrame.y);
		Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
		clips.push_back(clip);

		shared_ptr<Action> action = make_shared<Action>(clips, "BOMB1", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;

		for (int i = 0; i < 2; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 5.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "BOMB2", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;

		for (int i = 1; i < 3; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 5.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "BOMB3", Action::LOOP);
		_actions.push_back(action);
	}
}

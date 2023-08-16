#include "framework.h"
#include "Player.h"
#include "Whip.h"

Player::Player()
{
	_col = make_shared<RectCollider>(_playerSize);
	_layDownCol = make_shared<RectCollider>(Vector2(60.0f, 40.0f));
	_feetCol = make_shared<CircleCollider>(15.0f);
	_headCol = make_shared<CircleCollider>(10.0f);
	_grabCol = make_shared<CircleCollider>(10.0f);

	_transform = make_shared<Transform>();
	_upTransform = make_shared<Transform>();
	_downTransform = make_shared<Transform>();

	_sprite = make_shared<Sprite_Frame>(L"Resource/Texture/char_yellow.png", Vector2(16, 16));
	_damagedBuffer = make_shared<DamagedBuffer>();
	_playerPS = ADD_PS(L"Shader/PlayerPS.hlsl");
	_sprite->SetPS(_playerPS);
	_whip = make_shared<Whip>();

	_transform->SetParent(_col->GetTransform());
	_transform->SetPosition(Vector2(0.0f, 15.0f));
	_upTransform->SetParent(_col->GetTransform());
	_upTransform->SetPosition(Vector2(0.0f, 300.0f));
	_downTransform->SetParent(_col->GetTransform());
	_downTransform->SetPosition(Vector2(0.0f, -300.0f));
	
	_layDownCol->GetTransform()->SetParent(_col->GetTransform());
	_layDownCol->GetTransform()->SetPosition(Vector2(0.0f, -20.0f));
	_feetCol->GetTransform()->SetParent(_col->GetTransform());
	_feetCol->GetTransform()->SetPosition(Vector2(0.0f, -40.0f));
	_headCol->GetTransform()->SetParent(_col->GetTransform());
	_headCol->GetTransform()->SetPosition(Vector2(0.0f, 40.0f));
	_grabCol->GetTransform()->SetParent(_col->GetTransform());
	_grabCol->GetTransform()->SetPosition(Vector2(25.0f, 30.0f));

	_whip->GetTransform()->SetParent(_col->GetTransform());

	_col->GetTransform()->SetPosition(Vector2(0.1f, 0.1f));

	CreateAction();

	SetAction(State::IDLE);
}

Player::~Player()
{
}

void Player::Input()
{
	if (_isStun == true)
		return;

	if (KEY_DOWN('X'))
	{
		Attack();
	}

	if (_isClimb == true && _canClimb == true)
	{
		ClimbRadder();
		return;
	}

	if (_isGrabLedge == true)
	{
		GrabLedge();
		return;
	}

	if (KEY_PRESS(VK_LEFT))
	{
		//_col->GetTransform()->SetScale(Vector2(-1, +1));
		_sprite->SetLeft();
		_whip->SetLeft();
		_grabCol->GetTransform()->SetPosition(Vector2(-25.0f, 30.0f));
		_isLeft = true;
	}
	if (KEY_PRESS(VK_RIGHT))
	{
		//_col->GetTransform()->SetScale(Vector2(+1, +1));
		_sprite->SetRight();
		_whip->SetRight();
		_grabCol->GetTransform()->SetPosition(Vector2(25.0f, 30.0f));
		_isLeft = false;
	}

	if (KEY_PRESS(VK_LEFT))
	{
		if (KEY_PRESS(VK_DOWN) && _isFalling == false)
		{
			_curSpeed = 0.0f;
			_col->GetTransform()->AddVector2(-RIGHT_VECTOR * 150.0f * DELTA_TIME);
		}
		else if (_isPush == true)
		{
			_col->GetTransform()->AddVector2(-RIGHT_VECTOR * 101.0F * DELTA_TIME);
		}
		else
		{
			_curSpeed -= 30.0f;
			if (_curSpeed < -_moveSpeed)
				_curSpeed = -_moveSpeed;
		}
	}
	if (KEY_PRESS(VK_RIGHT))
	{
		if (KEY_PRESS(VK_DOWN) && _isFalling == false)
		{
			_curSpeed = 0.0f;
			_col->GetTransform()->AddVector2(RIGHT_VECTOR * 150.0f * DELTA_TIME);
		}
		else if (_isPush == true)
		{
			_col->GetTransform()->AddVector2(RIGHT_VECTOR * 101.0F * DELTA_TIME);
		}
		else
		{
			_curSpeed += 30.0f;
			if (_curSpeed > _moveSpeed)
				_curSpeed = _moveSpeed;
		}
	}

	if (!KEY_PRESS(VK_LEFT) && !KEY_PRESS(VK_RIGHT))
	{
		_curSpeed *= 0.9f;
		if (_curSpeed < 10.0f && _curSpeed > -10.0f)
			_curSpeed = 0.0f;
	}

	if (_curState == State::JUMP)
		return;

	if (KEY_PRESS(VK_UP) && _isFalling == false && _curState != State::RUN)
	{
		if (_isAttack == true)
			EndAttack();

		SetAction(State::LOOK_UP);
	}
	else if (KEY_UP(VK_UP) && _curState != State::RUN)
	{
		SetAction(State::IDLE);
	}

	if (KEY_PRESS(VK_DOWN) && _isFalling == false)
	{
		if (_isAttack == true)
			EndAttack();

		if (_curState != State::CRAWL)
			SetAction(State::LAY_DOWN);

		if (KEY_PRESS(VK_LEFT))
		{
			SetAction(State::CRAWL);
		}
		else if (KEY_UP(VK_LEFT))
			SetAction(State::LAY_DOWN);

		if (KEY_PRESS(VK_RIGHT))
		{
			SetAction(State::CRAWL);
		}
		else if (KEY_UP(VK_RIGHT))
			SetAction(State::LAY_DOWN);

		return;
	}
	else if (KEY_UP(VK_DOWN) && _isFalling == false)
	{
		SetAction(State::IDLE);
	}

	if (_curState == State::ATTACK)
		return;

	if (_isPush == true)
	{
		SetAction(State::PUSH);
		return;
	}

	if (_curSpeed <= 15.0f && _curSpeed >= -15.0f && !KEY_PRESS(VK_LEFT) && !KEY_PRESS(VK_RIGHT))
	{
		if (_curState != State::LOOK_UP)
			SetAction(State::IDLE);
	}
	else
		SetAction(State::RUN);
}

void Player::Jump()
{
	if (_isFalling == true && _isAttack == false && _isStun == false)
		SetAction(State::JUMP);
	else if (_curState == State::JUMP && _isFalling == false && _isAttack == false && _isStun == false)
		SetAction(State::IDLE);

	if (_actions[State::JUMP]->GetCurIndex() == 7)
		_actions[State::JUMP]->Pause();

	if (_isFalling == true)
		_jumpPower -= GRAVITY * 8;
	else
		_jumpPower = 0.0f;

	_col->GetTransform()->AddVector2(Vector2(0.0f, _jumpPower * DELTA_TIME));

	if (_jumpPower < -_maxFalling)
		_jumpPower = -_maxFalling;

	if (_onOneWay == true)
	{
		if (KEY_PRESS(VK_DOWN) && KEY_DOWN('Z'))
		{
			return;
		}
	}

	if (KEY_DOWN('Z') && _isFalling == false && _isStun == false)
	{
		_jumpPower = 1200.0f;
		_isFalling = true;
		_col->GetTransform()->AddVector2(Vector2(0.0f, 0.01f));
	}
}

void Player::Attack()
{
	if (_isClimb == true)
		_isClimb = false;

	if (_isGrabLedge == true)
		_isGrabLedge = false;

	if (_curState == State::LAY_DOWN || _curState == State::CRAWL)
		return;

	_whip->Attack();

	if (_isAttack == true)
	{
		_actions[State::ATTACK]->Pause();
		_actions[State::ATTACK]->Reset();
		_actions[State::ATTACK]->Play();
	}
	SetAction(State::ATTACK);
	_isAttack = true;
}

void Player::ClimbRadder()
{
	if (_canClimb == false)
		return;

	SetAction(State::CLIMB_RADDER);
	_isAttack = false;
	_isFalling = false;
	_whip->End();
	_jumpPower = 0.0f;

	if(KEY_DOWN(VK_UP) || KEY_DOWN(VK_DOWN))
		_actions[State::CLIMB_RADDER]->Play();

	if (KEY_PRESS(VK_UP))
	{
		_actions[State::CLIMB_RADDER]->SetReverse(false);
		_col->GetTransform()->AddVector2(UP_VECTOR * 150.0f * DELTA_TIME);
	}
	else if (KEY_UP(VK_UP))
	{
		_actions[State::CLIMB_RADDER]->Pause();
	}

	if (KEY_PRESS(VK_DOWN))
	{
		_actions[State::CLIMB_RADDER]->SetReverse(true);
		_col->GetTransform()->AddVector2(-UP_VECTOR * 150.0f * DELTA_TIME);
	}
	else if (KEY_UP(VK_DOWN))
	{
		_actions[State::CLIMB_RADDER]->Pause();
	}

	if (KEY_DOWN('Z'))
	{
		_isClimb = false;
		_canClimb = false;
		Jump();
	}
}

void Player::GrabLedge()
{
	SetAction(State::LEDGE_GRAB);

	_isAttack = false;
	_isFalling = false;
	_whip->End();

	if (KEY_DOWN('Z'))
	{
		_isGrabLedge = false;
		Jump();
	}
	else if (KEY_PRESS(VK_DOWN))
	{
		_isGrabLedge = false;
	}
	else if (_isLeft == true && KEY_DOWN(VK_RIGHT))
	{
		_isGrabLedge = false;
	}
	else if (_isLeft == false && KEY_DOWN(VK_LEFT))
	{
		_isGrabLedge = false;
	}
}

void Player::Update()
{
	if (_isStun == true)
	{
		if (_isFalling == true)
		{
			SetAction(State::STUN_AIR);
			_actions[State::STUN_AIR]->Pause();
			if (_jumpPower > 0)
			{
				_actions[State::STUN_AIR]->GetCurIndex() = 3;
			}
			else
			{
				_actions[State::STUN_AIR]->GetCurIndex() = 2;
			}
		}
		else
		{
			SetAction(State::STUN_GROUND);
		}
	}
	Input();
	Jump();
	if (_canClimb == false)
	{
		_climbTime += DELTA_TIME;
		_isClimb = false;
	}
	if (_climbTime >= 0.3f)
	{
		_canClimb = true;
		_climbTime = 0.0f;
	}

	if (_curState == State::LOOK_UP || _curState == State::LAY_DOWN)
	{
		_lookTime += DELTA_TIME;
	}
	else
	{
		_lookTime = 0.0f;
	}

	if (_lookTime >= 0.7f)
	{
		if (_curState == State::LOOK_UP)
		{
			//CAMERA->SetTarget(_upTransform);
		}
		else if (_curState == State::LAY_DOWN)
		{
			//CAMERA->SetTarget(_downTransform);
		}
	}
	else
	{
		//CAMERA->SetTarget(_col->GetTransform());
	}

	if (_isDamaged == true)
	{
		_curDamagedTime += DELTA_TIME;
	}
	if (_curDamagedTime >= _damagedCoolTime)
	{
		_isDamaged = false;
		_curDamagedTime = 0.0f;
	}

	if (_isDead == false)
	{
		if (_isStun == true)
		{
			_curStunTime += DELTA_TIME;
		}
		if (_curStunTime >= _stunCoolTime)
		{
			_isStun = false;
			_curStunTime = 0.0f;
		}
	}

	_col->GetTransform()->AddVector2(RIGHT_VECTOR * _curSpeed * DELTA_TIME);
	_whip->Update();
	_col->Update();
	_layDownCol->Update();
	_feetCol->Update();
	_headCol->Update();
	_grabCol->Update();
	_transform->Update();
	_upTransform->Update();
	_downTransform->Update();
	_actions[_curState]->Update();
	_damagedBuffer->_data.damaged = _isDamaged;
	_damagedBuffer->Update_Resource();

	_sprite->SetCurClip(_actions[_curState]->GetCurClip());
	_sprite->Update();
}

void Player::Render()
{
	_whip->Render();
	_transform->SetWorldBuffer(0);
	_damagedBuffer->SetPS_Buffer(2);
	_sprite->Render();
	_layDownCol->Render();
	_feetCol->Render();
	_headCol->Render();
	_grabCol->Render();
	_col->Render();
}

void Player::PostRender()
{
	ImGui::Text("x pos : %f", _col->GetWorldPos().x);
	ImGui::Text("y pos : %f", _col->GetWorldPos().y);
	ImGui::Text("jump power : %f", _jumpPower);
	ImGui::Text("is falling : %d", _isFalling);
	ImGui::Text("hp : %d", _hp);
	ImGui::Text("is Damaged : %d", _isDamaged);
	if (ImGui::Button("Kill", { 50,50 }))
	{
		Dead();
	}
}

void Player::SetAction(State state)
{
	if (_curState == state)
		return;

	_oldState = _curState;
	_actions[_oldState]->Reset();
	_actions[_oldState]->Pause();

	_curState = state;
	_actions[_curState]->Play();
}

void Player::SetIdle()
{
	if (_isAttack == true)
		return;
	SetAction(State::IDLE);
}

void Player::EndAttack()
{
	_isAttack = false;
	_whip->End();
	if (_isFalling == false)
		SetAction(State::IDLE);
	else
	{
		SetAction(State::JUMP);
	}
}

void Player::TakeDamage(int value)
{
	if (_isDead == true)
		return;

	if (_isDamaged == true)
		return;

	_hp -= value;
	if (_hp <= 0)
	{
		_hp = 0;
		Dead();
	}
	if (_isStun == true)
	{
		_curDamagedTime = 0.0f;
	}

	if(value > 1)
	{
		_isStun = true;
	}
	_isDamaged = true;
}

void Player::KnockBack(Vector2 pos, float value)
{
	if (_isDead == true)
		return;
	if (_isDamaged == true)
		return;

	if (_isGrabLedge == true)
		_isGrabLedge = false;
	if (_isClimb == true)
		_isClimb = false;

	_jumpPower = value;
	_isFalling = true;
	_col->GetTransform()->AddVector2(Vector2(0.0f, 0.01f));

	if (_col->GetWorldPos().x < pos.x)
	{
		_curSpeed = -value;
	}
	else
	{
		_curSpeed = value;
	}
}

void Player::StepOn(shared_ptr<Monster> monster)
{

}

void Player::Dead()
{
	_isStun = true;
	_isDead = true;
	_curSpeed = 0.0f;
	_hp = 0;
}

shared_ptr<RectCollider> Player::GetHitCollider()
{
	if (_isLaying == true)
	{
		return _layDownCol;
	}
	else
	{
		return _col;
	}
}

void Player::CreateAction()
{
	shared_ptr<SRV> srv = ADD_SRV(L"Resource/Texture/char_yellow.png");
	Vector2 imageSize = srv->GetImageSize();
	Vector2 maxFrame = Vector2(16, 16);
	Vector2 size;
	size.x = imageSize.x / maxFrame.x;
	size.y = imageSize.y / maxFrame.y;

	{
		vector<Action::Clip> clips;
		for (int i = 0; i < 1; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 0.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "IDLE", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 1; i < 9; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 0.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "RUN", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 2; i < 3; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 1.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "LAY_DOWN", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 5; i < 12; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 1.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "CRAWL", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 0; i < 4; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 2.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "STUN_AIR", Action::END);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 0; i < 8; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 9.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "JUMP", Action::END);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 0; i < 6; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 4.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "ATTACK", Action::END);
		action->SetEndEvent(std::bind(&Player::EndAttack, this));
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 0; i < 6; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 6.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "CLIMB_RADDER", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 11; i < 12; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 3.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "LEDGE_GRAB", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 3; i < 4; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 8.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "LOOK_UP", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 9; i < 10; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 0.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "STUN_GROUND", Action::LOOP);
		_actions.push_back(action);
	}

	{
		vector<Action::Clip> clips;
		for (int i = 6; i < 12; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 6.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		shared_ptr<Action> action = make_shared<Action>(clips, "PUSH", Action::LOOP);
		_actions.push_back(action);
	}
}

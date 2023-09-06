#include "framework.h"
#include "Rope.h"

Rope::Rope()
{
	_transform = make_shared<Transform>();
	_col = make_shared<RectCollider>(Vector2(20.0f, 100.0f));

	for (int i = 0; i < 4; i++)
	{
		shared_ptr<RectCollider> col = make_shared<RectCollider>(Vector2(20.0f, 100.0f));
		col->GetTransform()->SetParent(_col->GetTransform());
		col->GetTransform()->SetPosition(Vector2(0.0f, -100.0f * (i + 1)));
		_ropeCols.push_back(col);

	}

	_transform->SetParent(_col->GetTransform());

	_name = "Hook1";
	_isFalling = true;
	_jumpPower = 2000.0f;

	CreateAction();
}

Rope::~Rope()
{
}

void Rope::Update()
{
	_action->Update();
	for (auto col : _ropeCols)
		col->Update();
	if(_hooked == false)
		Item::Update();

	if (_jumpPower <= 0.0f)
	{
		if (_hooked == false)
			DropRope();
		_col->GetTransform()->SetPosition({ _col->GetWorldPos().x, MathUtility::GetGridPosition(_col->GetWorldPos()).y });
		_transform->Update();
		_col->Update();
		_name = "Hook2";
		_hooked = true;
	}
}

void Rope::Render()
{
	if (_isActive == false)
		return;
	if (_col->IsCollision(CAMERA->GetViewCollider()) == true)
	{
		_transform->SetPosition(Vector2(0.0f, 0.0f));
		_transform->Update();
		_transform->SetWorldBuffer(0);
		SPRITEMANAGER->Render("Rope", _name);
	}

	if (_hooked == true)
	{
		for (int i = 0; i < _curLength; i++)
		{
			_transform->SetPosition(Vector2(0.0f, -100.0f * (i + 1)));
			if (CAMERA->GetViewCollider()->IsCollision(_ropeCols[i]) == false)
				continue;
			_transform->Update();
			_transform->SetWorldBuffer(0);

			if (i == _curLength - 1)
			{
				if(i == _length - 1 && _dropEnd == true)
					SPRITEMANAGER->Render("Rope", "RopeEnd");
				else
					SPRITEMANAGER->Render("Rope", _action->GetCurClip());
			}
			else
				SPRITEMANAGER->Render("Rope", "Rope");
		}
	}
	//_col->Render();
}

void Rope::DropRope()
{
	if (_curLength >= _length)
	{
		_dropEnd = true;
		return;
	}
	_action->Play();
	_curLength++;
}

vector<shared_ptr<RectCollider>> Rope::GetColliders()
{
	return _ropeCols;
}

void Rope::CreateAction()
{
	shared_ptr<SRV> srv = ADD_SRV(L"Resource/Texture/char_yellow.png");
	Vector2 imageSize = srv->GetImageSize();
	Vector2 maxFrame = Vector2(16, 16);
	Vector2 size;
	size.x = imageSize.x / maxFrame.x;
	size.y = imageSize.y / maxFrame.y;

	{
		vector<Action::Clip> clips;
		for (int i = 1; i < 5; i++)
		{
			Vector2 startPos = Vector2((i * imageSize.x) / maxFrame.x, imageSize.y * 12.0f / maxFrame.y);
			Action::Clip clip = Action::Clip(startPos.x, startPos.y, size.x, size.y, srv);
			clips.push_back(clip);
		}

		_action = make_shared<Action>(clips, "DropRope", Action::END);
		_action->SetEndEvent(std::bind(&Rope::DropRope, this));
	}
}

#include "framework.h"
#include "Rope.h"

Rope::Rope()
{
	_transform = make_shared<Transform>();
	_col = make_shared<RectCollider>(Vector2(20.0f, 100.0f));

	_transform->SetParent(_col->GetTransform());

	_name = "Hook1";
	_isFalling = true;
	_jumpPower = 2000.0f;
}

Rope::~Rope()
{
}

void Rope::Update()
{
	if(_hooked == false)
		Item::Update();

	if (_jumpPower <= 0.0f)
	{
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
	if (_col->IsCollision(CAMERA->GetViewCollider()) == false)
		return;
	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Rope", _name);
	//_col->Render();
}

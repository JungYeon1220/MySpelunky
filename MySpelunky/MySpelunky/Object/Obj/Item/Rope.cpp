#include "framework.h"
#include "Rope.h"

Rope::Rope()
{
	_transform = make_shared<Transform>();
	_col = make_shared<RectCollider>(Vector2(50.0f, 100.0f));

	_transform->SetParent(_col->GetTransform());

	_name = "Hook1";
}

Rope::~Rope()
{
}

void Rope::Update()
{
	Item::Update();
}

void Rope::Render()
{
	if (_isActive == false)
		return;
	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Rope", _name);
}
